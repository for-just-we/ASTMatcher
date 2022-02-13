//
// Created by prophe on 2022/2/7.
//


#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/ExprConcepts.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Analysis/CFG.h"
#include "clang/Lex/Lexer.h"

#include "utils/UseAfterMoveUtils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;




class UAMMatcher: public MatchFinder::MatchCallback{
public:
    UAMMatcher(Rewriter &R): UAMRewriter(R){}

    void run(const MatchFinder::MatchResult &Result) override{
        const auto *ContainingLambda = Result.Nodes.getNodeAs<LambdaExpr>("containing-lambda");
        const auto *ContainingFunc = Result.Nodes.getNodeAs<FunctionDecl>("containing-func");
        const auto *CallMove = Result.Nodes.getNodeAs<CallExpr>("call-move");
        const auto *MovingCall = Result.Nodes.getNodeAs<Expr>("moving-call");
        const auto *Arg = Result.Nodes.getNodeAs<DeclRefExpr>("arg");

        if (!MovingCall || !MovingCall->getExprLoc().isValid())
            MovingCall = CallMove;

        Stmt *FunctionBody = nullptr;
        if (ContainingLambda)
            FunctionBody = ContainingLambda->getBody();
        else if (ContainingFunc)
            FunctionBody = ContainingFunc->getBody();
        else
            return;

        // Ignore the std::move if the variable that was passed to it isn't a local
        // variable.
        if (!Arg->getDecl()->getDeclContext()->isFunctionOrMethod())
            return;

        UseAfterMoveFinder Finder(Result.Context);
        UseAfterMove Use;

        bool flag = Finder.find(FunctionBody, MovingCall, Arg->getDecl(), &Use);
        if (flag)
            emitDiagnostic(MovingCall, Arg, Use, Result.Context);
    }
private:
    Rewriter UAMRewriter;
    SmallSet<FullSourceLoc, 8> EditedLocations;
};


//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class UAMASTConsumer : public ASTConsumer{
public:
    UAMASTConsumer(Rewriter &R, bool MainFileOnly=false) : UAMHandler(R){
        auto CallMoveMatcher =
                callExpr(callee(functionDecl(hasName("::std::move"))), argumentCountIs(1),
                         hasArgument(0, declRefExpr().bind("arg")),
                         anyOf(hasAncestor(lambdaExpr().bind("containing-lambda")),
                               hasAncestor(functionDecl().bind("containing-func"))),
                         unless(inDecltypeOrTemplateArg()))
                        .bind("call-move");
        // callExpr(callee(functionDecl(hasName("::std::move"))), argumentCountIs(1),hasArgument(0, declRefExpr().bind("arg")),anyOf(hasAncestor(lambdaExpr().bind("containing-lambda")),hasAncestor(functionDecl().bind("containing-func")))).bind("call-move")
        Finder.addMatcher(
                traverse(
                        TK_AsIs,
                        // To find the Stmt that we assume performs the actual move, we look
                        // for the direct ancestor of the std::move() that isn't one of the
                        // node types ignored by ignoringParenImpCasts().
                        stmt(
                                forEach(expr(ignoringParenImpCasts(CallMoveMatcher))),
                                // Don't allow an InitListExpr to be the moving call. An
                                // InitListExpr has both a syntactic and a semantic form, and the
                                // parent-child relationships are different between the two. This
                                // could cause an InitListExpr to be analyzed as the moving call
                                // in addition to the Expr that we actually want, resulting in two
                                // diagnostics with different code locations for the same move.
                                unless(initListExpr()),
                                unless(expr(ignoringParenImpCasts(equalsBoundNode("call-move")))))
                                .bind("moving-call")),
                &UAMHandler);
    }
    // stmt(forEach(expr(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::std::move"))), argumentCountIs(1),hasArgument(0, declRefExpr().bind("arg")),anyOf(hasAncestor(lambdaExpr().bind("containing-lambda")),hasAncestor(functionDecl().bind("containing-func")))).bind("call-move")))),unless(initListExpr()),unless(expr(ignoringParenImpCasts(equalsBoundNode("call-move"))))).bind("moving-call")
    void HandleTranslationUnit(ASTContext &Ctx) override {
        Finder.matchAST(Ctx);
    }
private:
    MatchFinder Finder;
    UAMMatcher UAMHandler;
};

//command line option, ASTMatcher testfile_path -- -I$LLVM_DIR/lib/clang/12.0.0/include

class UAMPluginAction : public PluginASTAction {
public:
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, clang::StringRef file) override {
        UAMRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<UAMASTConsumer>(UAMRewriter);
    }
private:
    Rewriter UAMRewriter;
};