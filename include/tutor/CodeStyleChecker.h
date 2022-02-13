//
// Created by prophe on 2022/2/2.
//
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/CharInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace llvm;

class CodeStyleCheckerVisitor: public clang::RecursiveASTVisitor<CodeStyleCheckerVisitor> {
public:
    explicit CodeStyleCheckerVisitor(clang::ASTContext *Ctx) : Ctx(Ctx) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl *Decl){
        if (Decl->getNameAsString().size() == 0)
            return true;
        checkNameStartsWithUpperCase(Decl);
        checkNoUnderscoreInName(Decl);
        return true;
    }

    bool VisitFunctionDecl(clang::FunctionDecl *Decl){
        // Skip user-defined conversion operators/functions:
        //    * https://en.cppreference.com/w/cpp/language/cast_operator
        if (clang::isa<clang::CXXConversionDecl>(Decl))
            return true;
        checkNameStartsWithLowerCase(Decl);
        checkNoUnderscoreInName(Decl);
        return true;
    }

    bool VisitVarDecl(clang::VarDecl *Decl){
        // Skip anonymous function parameter declarations
        if (clang::isa<clang::ParmVarDecl>(Decl) && (Decl->getNameAsString().size() == 0))
            return true;
        checkNameStartsWithUpperCase(Decl);
        checkNoUnderscoreInName(Decl);
        return true;
    }
    bool VisitFieldDecl(clang::FieldDecl *Decl){
        // Skip anonymous bit-fields:
        //  * https://en.cppreference.com/w/c/language/bit_field
        if (Decl->getNameAsString().size() == 0)
            return true;
        checkNameStartsWithUpperCase(Decl);
        checkNoUnderscoreInName(Decl);
        return true;
    }

private:
    clang::ASTContext *Ctx;

    // Checks whether the name in Decl contains any `_`. Issues a warnning if it
    // does.
    void checkNoUnderscoreInName(clang::NamedDecl *Decl){
        std::string Name = Decl->getNameAsString();
        size_t underscorePos = Name.find('_');

        if (underscorePos == llvm::StringRef::npos)
            return;

        std::string Hint = Name;
        std::string::iterator end_pos = std::remove(Hint.begin(), Hint.end(), '_');
        Hint.erase(end_pos, Hint.end());

        clang::FixItHint FixItHint = clang::FixItHint::CreateReplacement(
                clang::SourceRange(Decl->getLocation(),
                            Decl->getLocation().getLocWithOffset(Name.size())),
                Hint);

        clang::DiagnosticsEngine &DiagEngine = Ctx->getDiagnostics();
        unsigned DiagID = DiagEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                     "`_` in names is not allowed");
        clang::SourceLocation UnderscoreLoc =
                Decl->getLocation().getLocWithOffset(underscorePos);
        DiagEngine.Report(UnderscoreLoc, DiagID).AddFixItHint(FixItHint);
    }
    // Checks whether the name in Decl starts with a lower case letter. Issues a
    // warning if not.
    void checkNameStartsWithLowerCase(clang::NamedDecl *Decl){
        std::string Name = Decl->getNameAsString();
        char FirstChar = Name[0];

        // The actual check
        if (clang::isLowercase(FirstChar))
            return;

        // Construct the hint
        std::string Hint = Name;
        Hint[0] = clang::toLowercase(FirstChar);
        clang::FixItHint FixItHint = clang::FixItHint::CreateReplacement(
                clang::SourceRange(Decl->getLocation(),
                            Decl->getLocation().getLocWithOffset(Name.size())),
                Hint);

        // Construct the diagnostic
        clang::DiagnosticsEngine &DiagEngine = Ctx->getDiagnostics();
        unsigned DiagID = DiagEngine.getCustomDiagID(
                clang::DiagnosticsEngine::Warning,
                "Function names should start with lower-case letter");
        DiagEngine.Report(Decl->getLocation(), DiagID) << FixItHint;
    }

    // Checks whether the name in Decl starts with an upper case letter. Issues a
    // warning if not.
    void checkNameStartsWithUpperCase(clang::NamedDecl *Decl){
        std::string Name = Decl->getNameAsString();
        char FirstChar = Name[0];

        // The actual check
        if (clang::isUppercase(FirstChar))
            return;

        // Construct the hint
        std::string Hint = Name;
        Hint[0] = clang::toUppercase(FirstChar);
        clang::FixItHint FixItHint = clang::FixItHint::CreateReplacement(
                clang::SourceRange(Decl->getLocation(),
                            Decl->getLocation().getLocWithOffset(Name.size())),
                Hint);

        // Construct the diagnostic
        clang::DiagnosticsEngine &DiagEngine = Ctx->getDiagnostics();
        unsigned DiagID = DiagEngine.getCustomDiagID(
                clang::DiagnosticsEngine::Warning,
                "Type and variable names should start with upper-case letter");
        DiagEngine.Report(Decl->getLocation(), DiagID) << FixItHint;
    }
};

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class CodeStyleCheckerASTConsumer : public clang::ASTConsumer {
public:
    explicit CodeStyleCheckerASTConsumer(clang::ASTContext *Context,
                                         bool MainFileOnly,
                                         clang::SourceManager &SM)
            : Visitor(Context), SM(SM), MainTUOnly(MainFileOnly) {}

    void HandleTranslationUnit(clang::ASTContext &Ctx) {
        if (!MainTUOnly)
            Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
        else {
            // Only visit declarations declared in the input TU
            auto Decls = Ctx.getTranslationUnitDecl()->decls();
            for (auto &Decl : Decls) {
                // Ignore declarations out of the main translation unit.
                //
                // SourceManager::isInMainFile method takes into account locations
                // expansion like macro expansion scenario and checks expansion
                // location instead if spelling location if required.
                if (!SM.isInMainFile(Decl->getLocation()))
                    continue;
                Visitor.TraverseDecl(Decl);
            }
        }
    }

private:
    CodeStyleCheckerVisitor Visitor;
    clang::SourceManager &SM;
    // Should this plugin be only run on the main translation unit?
    bool MainTUOnly = true;
};

static cl::OptionCategory CSCCategory("code style checker options");

static cl::opt<bool> MainTuOnly{
        "main-tu-only",
        cl::desc("Only run on the main translation unit "
                 "(e.g. ignore included header files)"),
        cl::init(true), cl::cat(CSCCategory)
};

class CSCPluginAction : public clang::PluginASTAction {
public:
    bool ParseArgs(const clang::CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI,
                                                          clang::StringRef file) override {
        return std::make_unique<CodeStyleCheckerASTConsumer>(
                &CI.getASTContext(), MainTuOnly, CI.getSourceManager());
    }
};