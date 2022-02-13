//
// Created by prophe on 2022/2/1.
//

#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/StmtCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

static std::string NameToMatch("[U|u][N|n][U|u][S|s][E|e][D|d]");

namespace {
    AST_MATCHER_P(NamedDecl, matchesAnyListedName, std::string, Name) {
        return Regex(Name).match(Node.getName());
    }
} // namespace

//-----------------------------------------------------------------------------
// ASTMatcher callback
//-----------------------------------------------------------------------------
class UnusedForLoopVarMatcher: public MatchFinder::MatchCallback {
public:
    UnusedForLoopVarMatcher() = default;
    // Callback that's executed whenever the Matcher in
    // UnusedForLoopVarASTConsumer matches
    void run(const MatchFinder::MatchResult &Result) override{
        // ASTContext is used to retrieve the source location
        ASTContext *Ctx = Result.Context;
        if (Result.Nodes.getNodeAs<ForStmt>("RegularForLoop")) // C
            return runRegularForLoop(Result, Ctx);
        if (Result.Nodes.getNodeAs<CXXForRangeStmt>("RangeForLoop")) // C++
            return runRangeForLoop(Result, Ctx);
    }

private:
    void runRegularForLoop(const MatchFinder::MatchResult &Result, ASTContext *Ctx) {
        // Retrieve the matched nodes, C
        const ForStmt *RegularForLoop = Result.Nodes.getNodeAs<clang::ForStmt>("RegularForLoop");
        const DeclStmt *LoopVarRegularFor = Result.Nodes.getNodeAs<clang::DeclStmt>("LoopVarRegularFor");

        // Don't follow #include files
        if (RegularForLoop && !Ctx->getSourceManager().isInMainFile(RegularForLoop->getForLoc()))
            return;

        // Loop over all variables declared in the init statement of the loop
        for (auto *LoopVar: LoopVarRegularFor->decls()) {
            // If LoopVar _is used_ there's nothing to report
            if (LoopVar->isUsed(true))
                continue;
            // LoopVar is not used -> create a warning
            DiagnosticsEngine &DE = Ctx->getDiagnostics();
            unsigned DiagID = DE.getCustomDiagID(DiagnosticsEngine::Warning, "(AST Matcher) regular for-loop variable not used");
            auto DB = DE.Report(LoopVar->getLocation(), DiagID);
            DB.AddSourceRange(CharSourceRange::getCharRange(LoopVar->getSourceRange()));
        }
    }

    void runRangeForLoop(const MatchFinder::MatchResult &Result, clang::ASTContext *Ctx){
        // Retrieve the matched nodes, C++
        const CXXForRangeStmt *RangeForLoop = Result.Nodes.getNodeAs<CXXForRangeStmt>("RangeForLoop");
        const VarDecl *LoopVar = Result.Nodes.getNodeAs<VarDecl>("LoopVar");

        // Don't follow #include files: range for-loop
        if (RangeForLoop && !Ctx->getSourceManager().isInMainFile(RangeForLoop->getForLoc()))
            return;

        // If LoopVar _is used_ there's nothing to report
        if (LoopVar->isUsed(true))
            return;

        // LoopVar is not used -> create a warning
        DiagnosticsEngine &DE = Ctx->getDiagnostics();
        unsigned DiagID = DE.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                   "(AST Matcher) range for-loop variable not used");
        auto DB = DE.Report(LoopVar->getLocation(), DiagID);
        DB.AddSourceRange(CharSourceRange::getCharRange(LoopVar->getSourceRange()));
    }
};

//-----------------------------------------------------------------------------
// RecursiveASTVisitor
//-----------------------------------------------------------------------------
class UnusedForLoopVarVisitor: public RecursiveASTVisitor<UnusedForLoopVarVisitor> {
public:
    explicit UnusedForLoopVarVisitor(ASTContext *Ctx) : Ctx(Ctx) {}
    bool TraverseForStmt(ForStmt *S){
        auto InitStmt = dyn_cast<DeclStmt>(S->getInit());
        for (auto *InitVar : InitStmt->decls()) {
            const VarDecl *InitVarDecl = nullptr;
            InitVarDecl = dyn_cast<VarDecl>(InitVar);
            if (InitVarDecl && !Regex(NameToMatch).match(InitVarDecl->getName()))
                VarsDeclaredInLoopInitStmt.push_back(InitVarDecl);
        }

        // If there are no variables declared in the init statement, return true to
        // continue traversing the AST.
        if (VarsDeclaredInLoopInitStmt.empty())
            return true;
        // Traverse the body of the loop and record every use of the loop indices
        auto Ret = RecursiveASTVisitor::TraverseStmt(S->getBody());
        // For every loop variable declaration, LoopVarDecl, check whether the
        // corresponding variable is used?
        for (auto LoopVarDecl : VarsDeclaredInLoopInitStmt) {
            // LoopVar not in used var list
            if (UsedVars.erase(LoopVarDecl) == 0) {
                DiagnosticsEngine &DE = Ctx->getDiagnostics();
                unsigned DiagID = DE.getCustomDiagID(
                        DiagnosticsEngine::Warning,
                        "(Recursive AST Visitor) regular for-loop variable not used");
                auto DB = DE.Report(LoopVarDecl->getLocation(), DiagID);
                DB.AddSourceRange(CharSourceRange::getCharRange(LoopVarDecl->getSourceRange()));
            }
        }

        // Clean-up before visiting the next loop
        VarsDeclaredInLoopInitStmt.clear();
        UsedVars.clear();
        // TODO: What are we returning here?
        return Ret;
    }

    bool VisitDeclRefExpr(DeclRefExpr const *Stmt){
        // Get the declaration of the variable being referenced
        auto ReferencedVarDecl = dyn_cast<VarDecl>(Stmt->getDecl());

        // TODO - when do we not hit this?
        if (!ReferencedVarDecl)
            return true;

        // If this is a variable declared in the loop's init statement, record this for later.
        if (std::find(VarsDeclaredInLoopInitStmt.begin(),
                      VarsDeclaredInLoopInitStmt.end(),
                      ReferencedVarDecl) != VarsDeclaredInLoopInitStmt.end())
            UsedVars.insert(ReferencedVarDecl);

        return true;
    }

private:
    ASTContext *Ctx;

    SmallVector<clang::VarDecl const *, 4> VarsDeclaredInLoopInitStmt;
    SmallSet<clang::VarDecl const *, 4> UsedVars;
};


//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class UnusedForLoopVarASTConsumer : public ASTConsumer {
    public:
        UnusedForLoopVarASTConsumer(ASTContext &CTX, SourceManager &InSM): SM(InSM), UFLVVisitor(&CTX){
            // ASTMatcher for a range C++ for-loop
            auto LoopVar = varDecl(unless(matchesAnyListedName(NameToMatch)));
            StatementMatcher For = cxxForRangeStmt(hasLoopVariable(LoopVar.bind("LoopVar"))).bind("RangeForLoop");

            Matcher.addMatcher(For, &UFLVMatcher);

            // ASTMatcher for a regular C for-loop
            const auto LoopVarsInit =
                    declStmt(unless(forEach(varDecl(matchesAnyListedName(NameToMatch)))))
                            .bind("LoopVarRegularFor");
            auto ForStmt = forStmt(hasLoopInit(LoopVarsInit)).bind("RegularForLoop");
            // forStmt(hasLoopInit(declStmt().bind("LoopVarRegularFor"))).bind("RegularForLoop")
            Matcher.addMatcher(ForStmt, &UFLVMatcher);
        }

    void HandleTranslationUnit(ASTContext &Ctx) override {
        Matcher.matchAST(Ctx);

        // Only visit declarations declared in the input TU
        auto Decls = Ctx.getTranslationUnitDecl()->decls();
        for (auto &Decl : Decls) {
            if (!SM.isInMainFile(Decl->getLocation()))
                continue;
            UFLVVisitor.TraverseDecl(Decl);
        }
    }

private:
    MatchFinder Matcher;
    SourceManager &SM;

    UnusedForLoopVarMatcher UFLVMatcher;
    UnusedForLoopVarVisitor UFLVVisitor;
};


//-----------------------------------------------------------------------------
// FrontendAction
//-----------------------------------------------------------------------------
class UFLVMatcherPluginAction : public PluginASTAction {
    public:
        // Our plugin can alter behavior based on the command line options
        bool ParseArgs(const CompilerInstance &, const std::vector<std::string> &) override {
            return true;
        }

        // Returns our ASTConsumer per translation unit.
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
            return std::make_unique<UnusedForLoopVarASTConsumer>(CI.getASTContext(), CI.getSourceManager());
        }
};
