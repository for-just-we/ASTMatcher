//
// Created by prophe on 2022/2/3.
//

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;
//-----------------------------------------------------------------------------
// ASTMatcher callback
//-----------------------------------------------------------------------------
class LACommenterMatcher: public MatchFinder::MatchCallback {
public:
    LACommenterMatcher(Rewriter &LACRewriter) : LACRewriter(LACRewriter) {}
    // Callback that's executed whenever the Matcher in LACommenterASTConsumer
    // matches.
    void run(const MatchFinder::MatchResult &Result) override{
        // ASTContext is used to retrieve the source location
        ASTContext *Ctx = Result.Context;

        // Callee and caller are accessed via .bind("callee") and .bind("caller"),
        // respectively, from the ASTMatcher
        const FunctionDecl *CalleeDecl = Result.Nodes.getNodeAs<FunctionDecl>("callee");
        const CallExpr *TheCall = Result.Nodes.getNodeAs<CallExpr>("caller");

        // Basic sanity checking
        assert(TheCall && CalleeDecl &&
               "The matcher matched, so callee and caller should be non-null");

        // No arguments means there's nothing to comment
        if (CalleeDecl->parameters().empty())
            return;

        // Get the arguments
        Expr const *const *Args = TheCall->getArgs();
        size_t NumArgs = TheCall->getNumArgs();

        // If this is a call to an overloaded operator (e.g. `+`), then the first
        // parameter is the object itself (i.e. `this` pointer). Skip it.
        if (isa<CXXOperatorCallExpr>(TheCall)) {
            Args++;
            NumArgs--;
        }

        // For each argument match it with the callee parameter. If it is an integer,
        // float, boolean, character or string literal insert a comment.
        for (unsigned Idx = 0; Idx < NumArgs; Idx++) {
            const Expr *AE = Args[Idx]->IgnoreParenCasts();

            if (!dyn_cast<IntegerLiteral>(AE) && !dyn_cast<CXXBoolLiteralExpr>(AE) &&
                !dyn_cast<FloatingLiteral>(AE) && !dyn_cast<clang::StringLiteral>(AE) &&
                !dyn_cast<CharacterLiteral>(AE))
                continue;

            // Parameter declaration
            ParmVarDecl *ParamDecl = CalleeDecl->parameters()[Idx];

            // Source code locations (parameter and argument)
            FullSourceLoc ParamLocation = Ctx->getFullLoc(ParamDecl->getBeginLoc());
            FullSourceLoc ArgLoc = Ctx->getFullLoc(AE->getBeginLoc());

            if (ParamLocation.isValid() && !ParamDecl->getDeclName().isEmpty() && EditedLocations.insert(ArgLoc).second)
                // Insert the comment immediately before the argument
                LACRewriter.InsertText(ArgLoc,(Twine("/*") + ParamDecl->getDeclName().getAsString() + "=*/").str());
        }
    }
    // Callback that's executed at the end of the translation unit
    void onEndOfTranslationUnit() override{
        // Output to stdout
        LACRewriter.getEditBuffer(LACRewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
    }

private:
    clang::Rewriter LACRewriter;
    llvm::SmallSet<clang::FullSourceLoc, 8> EditedLocations;
};

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class LACommenterASTConsumer : public ASTConsumer {
public:
    LACommenterASTConsumer(Rewriter &R): LACHandler(R){
        /*
        1.Excludes function pointers and variadic functions.
        2.Excludes C++ member calls where the member is dependent on a template parameter.
        3.Has an inner matcher against the callee that we can bind
         */
        StatementMatcher CallSiteMatcher =
                callExpr(
                        allOf(callee(functionDecl(unless(isVariadic())).bind("callee")),
                              unless(cxxMemberCallExpr(
                                      on(hasType(substTemplateTypeParmType())))),
                              anyOf(hasAnyArgument(ignoringParenCasts(cxxBoolLiteral())),
                                    hasAnyArgument(ignoringParenCasts(integerLiteral())),
                                    hasAnyArgument(ignoringParenCasts(stringLiteral())),
                                    hasAnyArgument(ignoringParenCasts(characterLiteral())),
                                    hasAnyArgument(ignoringParenCasts(floatLiteral())))))
                        .bind("caller");

        // LAC is the callback that will run when the ASTMatcher finds the pattern
        // above.
        Finder.addMatcher(CallSiteMatcher, &LACHandler);
    }

    void HandleTranslationUnit(ASTContext &Ctx) override {
        Finder.matchAST(Ctx);
    }

private:
    MatchFinder Finder;
    LACommenterMatcher LACHandler;
};


//===----------------------------------------------------------------------===//
// PluginASTAction
//===----------------------------------------------------------------------===//
class LACPluginAction : public PluginASTAction {
public:
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   llvm::StringRef file) override {
        LACRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<LACommenterASTConsumer>(LACRewriter);
    }

private:
    Rewriter LACRewriter;
};
