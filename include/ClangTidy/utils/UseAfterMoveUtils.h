//
// Created by prophe on 2022/2/7.
//

#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/ExprConcepts.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Analysis/CFG.h"
#include "clang/Lex/Lexer.h"
#include "ExprSequence.h"

using namespace clang::ast_matchers;

namespace {
    AST_MATCHER(Expr, hasUnevaluatedContext) {
        if (isa<CXXNoexceptExpr>(Node) || isa<RequiresExpr>(Node))
            return true;
        if (const auto *UnaryExpr = dyn_cast<UnaryExprOrTypeTraitExpr>(&Node)) {
            switch (UnaryExpr->getKind()) {
                case UETT_SizeOf:
                case UETT_AlignOf:
                    return true;
                default:
                    return false;
            }
        }
        if (const auto *TypeIDExpr = dyn_cast<CXXTypeidExpr>(&Node))
            return !TypeIDExpr->isPotentiallyEvaluated();
        return false;
    }
}

static StatementMatcher inDecltypeOrTemplateArg() {
    return anyOf(hasAncestor(typeLoc()),
                 hasAncestor(declRefExpr(
                         to(functionDecl(ast_matchers::isTemplateInstantiation())))),
                 hasAncestor(expr(hasUnevaluatedContext())));
}



/// Contains information about a use-after-move.
struct UseAfterMove {
    // The DeclRefExpr that constituted the use of the object.
    const DeclRefExpr *DeclRef;

    // Is the order in which the move and the use are evaluated undefined?
    bool EvaluationOrderUndefined;
};


/// Finds uses of a variable after a move (and maintains state required by the
/// various internal helper functions).
class UseAfterMoveFinder {
public:
    UseAfterMoveFinder(ASTContext *TheContext): Context(TheContext){}

    // Within the given function body, finds the first use of 'MovedVariable' that
    // occurs after 'MovingCall' (the expression that performs the move). If a
    // use-after-move is found, writes information about it to 'TheUseAfterMove'.
    // Returns whether a use-after-move was found.
    bool find(Stmt *FunctionBody, const Expr *MovingCall,
              const ValueDecl *MovedVariable, UseAfterMove *TheUseAfterMove);

private:
    bool findInternal(const CFGBlock *Block, const Expr *MovingCall,
                      const ValueDecl *MovedVariable,
                      UseAfterMove *TheUseAfterMove);
    void getUsesAndReinits(const CFGBlock *Block, const ValueDecl *MovedVariable,
                           llvm::SmallVectorImpl<const DeclRefExpr *> *Uses,
                           llvm::SmallPtrSetImpl<const Stmt *> *Reinits);
    void getDeclRefs(const CFGBlock *Block, const Decl *MovedVariable,
                     llvm::SmallPtrSetImpl<const DeclRefExpr *> *DeclRefs);
    void getReinits(const CFGBlock *Block, const ValueDecl *MovedVariable,
                    llvm::SmallPtrSetImpl<const Stmt *> *Stmts,
                    llvm::SmallPtrSetImpl<const DeclRefExpr *> *DeclRefs);

    ASTContext *Context;
    std::unique_ptr<ExprSequence> Sequence;
    std::unique_ptr<StmtToBlockMap> BlockMap;
    llvm::SmallPtrSet<const CFGBlock *, 8> Visited;
};

bool UseAfterMoveFinder::find(Stmt *FunctionBody, const Expr *MovingCall,
                              const ValueDecl *MovedVariable,
                              UseAfterMove *TheUseAfterMove) {
    // Generate the CFG manually instead of through an AnalysisDeclContext because
    // it seems the latter can't be used to generate a CFG for the body of a
    // lambda.
    //
    // We include implicit and temporary destructors in the CFG so that
    // destructors marked [[noreturn]] are handled correctly in the control flow
    // analysis. (These are used in some styles of assertion macros.)
    CFG::BuildOptions Options;
    Options.AddImplicitDtors = true;
    Options.AddTemporaryDtors = true;
    // 生成CFG
    std::unique_ptr<CFG> TheCFG = CFG::buildCFG(nullptr, FunctionBody, Context, Options);
    if (!TheCFG)
        return false;

    Sequence = std::make_unique<ExprSequence>(TheCFG.get(), FunctionBody, Context);
    BlockMap = std::make_unique<StmtToBlockMap>(TheCFG.get(), Context);
    Visited.clear();


    const CFGBlock *Block = BlockMap->blockContainingStmt(MovingCall);
    if (!Block) {
        // This can happen if MovingCall is in a constructor initializer, which is
        // not included in the CFG because the CFG is built only from the function
        // body.
        Block = &TheCFG->getEntry();
    }

    return findInternal(Block, MovingCall, MovedVariable, TheUseAfterMove);
}

bool UseAfterMoveFinder::findInternal(const CFGBlock *Block,
                                      const Expr *MovingCall,
                                      const ValueDecl *MovedVariable,
                                      UseAfterMove *TheUseAfterMove) {
    if (Visited.count(Block))
        return false;
    // Mark the block as visited (except if this is the block containing the
    // std::move() and it's being visited the first time).
    if (!MovingCall)
        Visited.insert(Block);


    // Get all uses and reinits in the block.
    llvm::SmallVector<const DeclRefExpr *, 1> Uses;
    llvm::SmallPtrSet<const Stmt *, 1> Reinits;
    // 获取该Block对应的关于MovedVariable的use的reinit情况
    getUsesAndReinits(Block, MovedVariable, &Uses, &Reinits);
    llvm::SmallVector<const Stmt *, 1> ReinitsToDelete;
    // 有的reinit可能在MovingCall之前，这个要排除
    for (const Stmt *Reinit : Reinits) {
        if (MovingCall && Sequence->potentiallyAfter(MovingCall, Reinit))
            ReinitsToDelete.push_back(Reinit);
    }
    for (const Stmt *Reinit : ReinitsToDelete) {
        Reinits.erase(Reinit);
    }

    // 寻找此Block中在MovingCall之后变量使用情况
    for (const DeclRefExpr *Use : Uses) {
        if (!MovingCall || Sequence->potentiallyAfter(Use, MovingCall)) {
            // Does the use have a saving reinit? A reinit is saving if it definitely
            // comes before the use, i.e. if there's no potential that the reinit is
            // after the use.
            bool HaveSavingReinit = false;
            for (const Stmt *Reinit : Reinits) {
                if (!Sequence->potentiallyAfter(Reinit, Use))
                    HaveSavingReinit = true;
            }

            if (!HaveSavingReinit) {
                TheUseAfterMove->DeclRef = Use;
                TheUseAfterMove->EvaluationOrderUndefined =
                        MovingCall != nullptr &&
                        Sequence->potentiallyAfter(MovingCall, Use);

                return true;
            }
        }
    }

    // If the object wasn't reinitialized, call ourselves recursively on all
    // successors.
    if (Reinits.empty()) {
        for (const auto &Succ : Block->succs()) {
            if (Succ && findInternal(Succ, nullptr, MovedVariable, TheUseAfterMove))
                return true;
        }
    }

    return false;
}

// 在一个CFG Block中进行查找操作
void UseAfterMoveFinder::getUsesAndReinits(
        const CFGBlock *Block, const ValueDecl *MovedVariable,
        llvm::SmallVectorImpl<const DeclRefExpr *> *Uses,
        llvm::SmallPtrSetImpl<const Stmt *> *Reinits) {
    llvm::SmallPtrSet<const DeclRefExpr *, 1> DeclRefs;
    llvm::SmallPtrSet<const DeclRefExpr *, 1> ReinitDeclRefs;

    getDeclRefs(Block, MovedVariable, &DeclRefs);
    getReinits(Block, MovedVariable, Reinits, &ReinitDeclRefs);

    // All references to the variable that aren't reinitializations are uses.
    Uses->clear();
    // use都是DeclRefExpr，但是DeclRefExpr集合可能包含了reinit的情况，因此要从use中排除这些情况
    for (const DeclRefExpr *DeclRef : DeclRefs) {
        if (!ReinitDeclRefs.count(DeclRef))
            Uses->push_back(DeclRef);
    }

    // Sort the uses by their occurrence in the source code.
    std::sort(Uses->begin(), Uses->end(),
              [](const DeclRefExpr *D1, const DeclRefExpr *D2) {
                  return D1->getExprLoc() < D2->getExprLoc();
              });
}

bool isStandardSmartPointer(const ValueDecl *VD) {
    const clang::Type *TheType = VD->getType().getNonReferenceType().getTypePtrOrNull();
    if (!TheType)
        return false;

    const CXXRecordDecl *RecordDecl = TheType->getAsCXXRecordDecl();
    if (!RecordDecl)
        return false;

    const IdentifierInfo *ID = RecordDecl->getIdentifier();
    if (!ID)
        return false;

    StringRef Name = ID->getName();
    if (Name != "unique_ptr" && Name != "shared_ptr" && Name != "weak_ptr")
        return false;

    return RecordDecl->getDeclContext()->isStdNamespace();
}

void UseAfterMoveFinder::getDeclRefs(
        const CFGBlock *Block, const Decl *MovedVariable,
        llvm::SmallPtrSetImpl<const DeclRefExpr *> *DeclRefs) {
    DeclRefs->clear();
    // 匹配该Block中的每个Elem
    for (const auto &Elem : *Block) {
        Optional<CFGStmt> S = Elem.getAs<CFGStmt>();
        if (!S)
            continue;
        // 定义匹配Stmt的Lambda函数
        auto AddDeclRefs = [this, Block,
                DeclRefs](const ArrayRef<BoundNodes> Matches) {
            for (const auto &Match : Matches) {
                const auto *DeclRef = Match.getNodeAs<DeclRefExpr>("declref");
                const auto *Operator = Match.getNodeAs<CXXOperatorCallExpr>("operator");
                if (DeclRef && BlockMap->blockContainingStmt(DeclRef) == Block) {
                    // Ignore uses of a standard smart pointer that don't dereference the
                    // pointer.
                    if (Operator || !isStandardSmartPointer(DeclRef->getDecl())) {
                        DeclRefs->insert(DeclRef);
                    }
                }
            }
        };

        auto DeclRefMatcher = declRefExpr(hasDeclaration(equalsNode(MovedVariable)),
                                          unless(inDecltypeOrTemplateArg()))
                .bind("declref");
        // 进行匹配操作
        AddDeclRefs(match(traverse(TK_AsIs, findAll(DeclRefMatcher)), *S->getStmt(),
                          *Context));
        AddDeclRefs(match(findAll(cxxOperatorCallExpr(
                                  hasAnyOverloadedOperatorName("*", "->", "[]"),
                                  hasArgument(0, DeclRefMatcher))
                                          .bind("operator")),
                          *S->getStmt(), *Context));
    }
}

void UseAfterMoveFinder::getReinits(
        const CFGBlock *Block, const ValueDecl *MovedVariable,
        llvm::SmallPtrSetImpl<const Stmt *> *Stmts,
        llvm::SmallPtrSetImpl<const DeclRefExpr *> *DeclRefs) {
    auto DeclRefMatcher =
            declRefExpr(hasDeclaration(equalsNode(MovedVariable))).bind("declref");

    auto StandardContainerTypeMatcher = hasType(hasUnqualifiedDesugaredType(
            recordType(hasDeclaration(cxxRecordDecl(hasAnyName(
                    "::std::basic_string", "::std::vector", "::std::deque",
                    "::std::forward_list", "::std::list", "::std::set", "::std::map",
                    "::std::multiset", "::std::multimap", "::std::unordered_set",
                    "::std::unordered_map", "::std::unordered_multiset",
                    "::std::unordered_multimap"))))));

    auto StandardSmartPointerTypeMatcher = hasType(hasUnqualifiedDesugaredType(
            recordType(hasDeclaration(cxxRecordDecl(hasAnyName(
                    "::std::unique_ptr", "::std::shared_ptr", "::std::weak_ptr"))))));

    // Matches different types of reinitialization.
    auto ReinitMatcher =
            stmt(anyOf(
                    // Assignment. In addition to the overloaded assignment operator,
                    // test for built-in assignment as well, since template functions
                    // may be instantiated to use std::move() on built-in types.
                    binaryOperation(hasOperatorName("="), hasLHS(DeclRefMatcher)),
                    // Declaration. We treat this as a type of reinitialization too,
                    // so we don't need to treat it separately.
                    declStmt(hasDescendant(equalsNode(MovedVariable))),
                    // clear() and assign() on standard containers.
                    cxxMemberCallExpr(
                            on(expr(DeclRefMatcher, StandardContainerTypeMatcher)),
                            // To keep the matcher simple, we check for assign() calls
                            // on all standard containers, even though only vector,
                            // deque, forward_list and list have assign(). If assign()
                            // is called on any of the other containers, this will be
                            // flagged by a compile error anyway.
                            callee(cxxMethodDecl(hasAnyName("clear", "assign")))),
                    // reset() on standard smart pointers.
                    cxxMemberCallExpr(
                            on(expr(DeclRefMatcher, StandardSmartPointerTypeMatcher)),
                            callee(cxxMethodDecl(hasName("reset")))),
                    // Methods that have the [[clang::reinitializes]] attribute.
                    cxxMemberCallExpr(
                            on(DeclRefMatcher),
                            callee(cxxMethodDecl(hasAttr(clang::attr::Reinitializes)))),
                    // Passing variable to a function as a non-const pointer.
                    callExpr(forEachArgumentWithParam(
                            unaryOperator(hasOperatorName("&"),
                                          hasUnaryOperand(DeclRefMatcher)),
                            unless(parmVarDecl(hasType(pointsTo(isConstQualified())))))),
                    // Passing variable to a function as a non-const lvalue reference
                    // (unless that function is std::move()).
                    callExpr(forEachArgumentWithParam(
                                     traverse(TK_AsIs, DeclRefMatcher),
                                     unless(parmVarDecl(hasType(
                                             references(qualType(isConstQualified())))))),
                             unless(callee(functionDecl(hasName("::std::move")))))))
                    .bind("reinit");

    Stmts->clear();
    DeclRefs->clear();
    for (const auto &Elem : *Block) {
        Optional<CFGStmt> S = Elem.getAs<CFGStmt>();
        if (!S)
            continue;
        // 匹配重新赋值操作
        SmallVector<BoundNodes, 1> Matches =
                match(findAll(ReinitMatcher), *S->getStmt(), *Context);
        // 遍历每个匹配结果
        for (const auto &Match : Matches) {
            const auto *TheStmt = Match.getNodeAs<Stmt>("reinit");
            const auto *TheDeclRef = Match.getNodeAs<DeclRefExpr>("declref");
            if (TheStmt && BlockMap->blockContainingStmt(TheStmt) == Block) {
                // 记录重新赋值操作的完整的Stmt
                Stmts->insert(TheStmt);
                // We count DeclStmts as reinitializations, but they don't have a
                // DeclRefExpr associated with them -- so we need to check 'TheDeclRef'
                // before adding it to the set.
                if (TheDeclRef){
                    // 记录一个变量结点DeclRefExpr
                    DeclRefs->insert(TheDeclRef);
                }

            }
        }
    }
}


static void emitDiagnostic(const Expr *MovingCall, const DeclRefExpr *MoveArg,
                           const UseAfterMove &Use, ASTContext *Context) {
    SourceLocation UseLoc = Use.DeclRef->getExprLoc();
    SourceLocation MoveLoc = MovingCall->getExprLoc();

    DiagnosticsEngine &DiagEngine = Context->getDiagnostics();
    unsigned DiagID = DiagEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                 "variable used after it was moved");
    DiagEngine.Report(UseLoc, DiagID);

    DiagID = DiagEngine.getCustomDiagID(DiagnosticsEngine::Warning, "move occurred here");
    DiagEngine.Report(MoveLoc, DiagID);

    if (Use.EvaluationOrderUndefined) {
        DiagID = DiagEngine.getCustomDiagID(DiagnosticsEngine::Warning, "the use and move are unsequenced, i.e. there is no guarantee \"\n"
                                                                        "                \"about the order in which they are evaluated");
        DiagEngine.Report(UseLoc, DiagID);
    } else if (UseLoc < MoveLoc || Use.DeclRef == MoveArg) {
        DiagID = DiagEngine.getCustomDiagID(DiagnosticsEngine::Warning, "the use happens in a later loop iteration than the move");
        DiagEngine.Report(UseLoc, DiagID);
    }
}