//
// Created by prophe on 2022/2/6.
//

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include <memory>
using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;

//-----------------------------------------------------------------------------
// ASTMatcher callback (add instructions)
//-----------------------------------------------------------------------------
class ObfuscatorMatcherForAdd: public MatchFinder::MatchCallback {
public:
    explicit ObfuscatorMatcherForAdd(std::shared_ptr<Rewriter> RewriterForObfuscator)
            : ObfuscatorRewriter{std::move(RewriterForObfuscator)} {}

    void run(const MatchFinder::MatchResult &Result) override{
        std::string LHSAsStr, RHSAsStr;
        SourceRange RangeLHS, RangeRHS;

        if (Result.Nodes.getMap().size() == 0)
            llvm_unreachable("Unsupported case in the handler for `+`");

        // Grab the left-hand-side of the expression
        if (const auto *LHSWithIC =
                Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("lhs")) {
            const auto *LHS = LHSWithIC->IgnoreImplicit();

            LHSAsStr = dyn_cast<clang::DeclRefExpr>(LHS)->getDecl()->getName().str();
            RangeLHS = LHS->getSourceRange();
        } else if (const auto *LHS =
                Result.Nodes.getNodeAs<clang::IntegerLiteral>("lhs")) {
            LHSAsStr = std::to_string(LHS->getValue().getZExtValue());
            RangeLHS = LHS->getSourceRange();
        } else {
            llvm_unreachable("Unsupported case in the handler for `+`");
        }

        // Grab the right-hand-side of the expression
        if (const auto *RHSWithIC =
                Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("rhs")) {
            const auto *RHS = RHSWithIC->IgnoreImplicit();

            RHSAsStr = dyn_cast<clang::DeclRefExpr>(RHS)->getDecl()->getName().str();
            RangeRHS = RHS->getSourceRange();
        } else if (const auto *RHS =
                Result.Nodes.getNodeAs<clang::IntegerLiteral>("rhs")) {
            RHSAsStr = std::to_string(RHS->getValue().getZExtValue());
            RangeRHS = RHS->getSourceRange();
        } else {
            llvm_unreachable("Unsupported case in the handler for `+`");
        }

        // Rewrite the expression
        ObfuscatorRewriter->ReplaceText(RangeLHS, "(" + LHSAsStr + " ^ " + RHSAsStr + ")");
        ObfuscatorRewriter->ReplaceText(RangeRHS, "2 * (" + LHSAsStr + " & " + RHSAsStr + ")");
    }

private:
    std::shared_ptr<Rewriter> ObfuscatorRewriter;
};

//-----------------------------------------------------------------------------
// ASTMatcher callback (sub instructions)
//-----------------------------------------------------------------------------
class ObfuscatorMatcherForSub: public MatchFinder::MatchCallback {
public:
    explicit ObfuscatorMatcherForSub(std::shared_ptr<Rewriter> RewriterForObfuscatorSub)
            : ObfuscatorRewriter{std::move(RewriterForObfuscatorSub)} {

    }

    void run(const MatchFinder::MatchResult &Result) override{
        const auto &Op = Result.Nodes.getNodeAs<BinaryOperator>("op");

        std::string LHSAsStr, RHSAsStr;
        SourceRange RangeLHS, RangeRHS;

        // Grab the left-hand-side of the expression
        if (const auto *LHSWithIC =
                Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("lhs")) {
            const auto *LHS = LHSWithIC->IgnoreImplicit();

            LHSAsStr = dyn_cast<DeclRefExpr>(LHS)->getDecl()->getName().str();
            RangeLHS = LHS->getSourceRange();
        } else if (const auto *LHS =
                Result.Nodes.getNodeAs<IntegerLiteral>("lhs")) {
            LHSAsStr = std::to_string(LHS->getValue().getZExtValue());
            RangeLHS = LHS->getSourceRange();
        } else {
            llvm_unreachable("Unsupported case in the handler for `-`");
        }

        // Grab the right-hand-side of the expression
        if (const auto *RHSWithIC =
                Result.Nodes.getNodeAs<ImplicitCastExpr>("rhs")) {
            const auto *RHS = RHSWithIC->IgnoreImplicit();

            RHSAsStr = dyn_cast<DeclRefExpr>(RHS)->getDecl()->getName().str();
            RangeRHS = RHS->getSourceRange();
        } else if (const auto *RHS =
                Result.Nodes.getNodeAs<IntegerLiteral>("rhs")) {
            RHSAsStr = std::to_string(RHS->getValue().getZExtValue());
            RangeRHS = RHS->getSourceRange();
        } else {
            llvm_unreachable("Unsupported case in the handler for `-`");
        }

        // Rewrite the expression
        ObfuscatorRewriter->ReplaceText(RangeLHS, "(" + LHSAsStr + " + ~" + RHSAsStr + ")");
        ObfuscatorRewriter->ReplaceText(Op->getOperatorLoc(), "+");
        ObfuscatorRewriter->ReplaceText(RangeRHS, "1");
    }

private:
    std::shared_ptr<Rewriter> ObfuscatorRewriter;
};

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class ObfuscatorASTConsumer : public ASTConsumer{
public:
    ObfuscatorASTConsumer(std::shared_ptr<Rewriter> R): AddHandler(R), SubHandler(R){
        const auto MatcherAdd =
                binaryOperator(
                        hasOperatorName("+"),
                        hasLHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("lhs"),
                                     integerLiteral().bind("lhs"))),
                        hasRHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("rhs"),
                                     integerLiteral().bind("rhs"))))
                        .bind("op");

        const auto MatcherSub =
                binaryOperator(
                        hasOperatorName("-"),
                        hasLHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("lhs"),
                                     integerLiteral().bind("lhs"))),
                        hasRHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("rhs"),
                                     integerLiteral().bind("rhs"))))
                        .bind("op");

        Matcher.addMatcher(MatcherAdd, &AddHandler);
        Matcher.addMatcher(MatcherSub, &SubHandler);
    }

    void HandleTranslationUnit(ASTContext &Ctx) override {
        Matcher.matchAST(Ctx);
    }

private:
    MatchFinder Matcher;
    ObfuscatorMatcherForAdd AddHandler;
    ObfuscatorMatcherForSub SubHandler;
};



//-----------------------------------------------------------------------------
// FrontendAction
//-----------------------------------------------------------------------------
class ObfuscatorPluginAction : public PluginASTAction {
public:
    ObfuscatorPluginAction(): RewriterForObfuscator{std::make_shared<Rewriter>()} {}
    // Our plugin can alter behavior based on the command line options
    bool ParseArgs(const CompilerInstance &,
                   const std::vector<std::string> &) override {
        return true;
    }

    // Returns our ASTConsumer per translation unit.
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   StringRef file) override {
        RewriterForObfuscator->setSourceMgr(CI.getSourceManager(),
                                            CI.getLangOpts());
        return std::make_unique<ObfuscatorASTConsumer>(RewriterForObfuscator);
    }

    void EndSourceFileAction() override {
        // Output to stdout (via llvm::outs()
        RewriterForObfuscator
                ->getEditBuffer(RewriterForObfuscator->getSourceMgr().getMainFileID())
                .write(llvm::outs());
    }

private:
    std::shared_ptr<Rewriter> RewriterForObfuscator;
};

/*
binaryOperator(hasOperatorName("+"),hasLHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("lhs"),integerLiteral().bind("lhs"))),hasRHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("rhs"),integerLiteral().bind("rhs")))).bind("op")
 */