//
// Created by prophe on 2022/1/30.
//
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <memory>

using namespace llvm;
// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class FunctionNameVisitor : public clang::RecursiveASTVisitor<FunctionNameVisitor> {
public:
    bool VisitFunctionDecl(clang::FunctionDecl *f) {
        // debug
        llvm::outs() << "Function "
                     << (f->hasBody() ? "Def" : "Decl")
                     << " "
                     << f->getNameInfo().getName()
                     << "\n";
        return true;
    }
};


class PrintFunctionsAction : public clang::PluginASTAction {
public:
    PrintFunctionsAction()
    { }

    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef) override {
        ci.getDiagnostics().setClient(new clang::IgnoringDiagConsumer());
        return std::make_unique<clang::ASTConsumer>();
    }

    bool ParseArgs(const clang::CompilerInstance &ci,
              const std::vector<std::string>& args) override {
        return true;
    }

protected:
    void EndSourceFileAction() override{
        clang::CompilerInstance &ci = getCompilerInstance();
        clang::ASTContext &context = ci.getASTContext();

        const clang::FrontendInputFile &input = getCurrentInput();
        llvm::StringRef fileName = input.getFile();
        llvm::outs() << "Filename in Action: " << fileName << "\n";

        clang::TranslationUnitDecl *unit = context.getTranslationUnitDecl();
        FunctionNameVisitor visitor;
        visitor.TraverseDecl(unit);

        clang::ASTFrontendAction::EndSourceFileAction();
    }

    clang::PluginASTAction::ActionType getActionType() override {
        return ReplaceAction;
    }
};