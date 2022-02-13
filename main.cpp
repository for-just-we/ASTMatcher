#include "tutor/PrintFunctionsAction.h"
#include "tutor/UnusedForLoopVar.h"
#include "tutor/CodeStyleChecker.h"
#include "tutor/LACommenter.h"
#include "tutor/Obfuscator.h"

#include "ClangTidy/UseAfterMoveCheck.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/FrontendActions.h"

#include<string>

using namespace clang::tooling;

std::string OpText = "CSC:CodeStyleChecker, LAC:LACommenter, OBF:Obfuscator, PFA:PrintFUnctions, UFL:UnusedForLoopVarChecker, UAM:UseAfterMoveChecker";
static cl::OptionCategory CheckerCategory("checker options");
static cl::opt<std::string> CheckerOp("checker",
                cl::desc(OpText),
                cl::cat(CheckerCategory));


int main(int argc, const char** argv) {
    CommonOptionsParser op(argc, argv, CheckerCategory);
    ClangTool tool{op.getCompilations(), op.getSourcePathList()};

    std::unique_ptr<FrontendActionFactory> FrontendFactory;
    if (CheckerOp == "CSC")
        FrontendFactory = newFrontendActionFactory<CSCPluginAction>();
    else if (CheckerOp == "LAC")
        FrontendFactory = newFrontendActionFactory<LACPluginAction>();
    else if (CheckerOp == "OBF")
        FrontendFactory = newFrontendActionFactory<ObfuscatorPluginAction>();
    else if (CheckerOp == "UFL")
        FrontendFactory = newFrontendActionFactory<UFLVMatcherPluginAction>();
    else if (CheckerOp == "UAM")
        FrontendFactory = newFrontendActionFactory<UAMPluginAction>();
    else
        FrontendFactory = newFrontendActionFactory<PrintFunctionsAction>();

    int status = tool.run(FrontendFactory.get());
    return status;
}