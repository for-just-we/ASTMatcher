//
// Created by prophe on 2022/2/11.
//

#include "clang/Analysis/CFG.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

static SmallVector<const Stmt *, 1> getParentStmts(const Stmt *S, ASTContext *Context) {
    SmallVector<const Stmt *, 1> Result;

    TraversalKindScope RAII(*Context, TK_AsIs);
    DynTypedNodeList Parents = Context->getParents(*S);

    SmallVector<DynTypedNode, 1> NodesToProcess(Parents.begin(), Parents.end());

    while (!NodesToProcess.empty()) {
        DynTypedNode Node = NodesToProcess.back();
        NodesToProcess.pop_back();

        if (const auto *S = Node.get<Stmt>()) {
            Result.push_back(S);
        } else {
            Parents = Context->getParents(Node);
            NodesToProcess.append(Parents.begin(), Parents.end());
        }
    }

    return Result;
}


bool isDescendantOrEqual(const Stmt *Descendant, const Stmt *Ancestor,
                         ASTContext *Context) {
    if (Descendant == Ancestor)
        return true;
    for (const Stmt *Parent : getParentStmts(Descendant, Context)) {
        if (isDescendantOrEqual(Parent, Ancestor, Context))
            return true;
    }

    return false;
}


class ExprSequence {
public:
    /// Initializes this `ExprSequence` with sequence information for the given
    /// `CFG`. `Root` is the root statement the CFG was built from.
    ExprSequence(const CFG *TheCFG, const Stmt *Root, ASTContext *TheContext): Context(TheContext), Root(Root){
        for (const auto &SyntheticStmt : TheCFG->synthetic_stmts()) {
            SyntheticStmtSourceMap[SyntheticStmt.first] = SyntheticStmt.second;
        }
    }

    /// Returns whether \p Before is sequenced before \p After.
    bool inSequence(const Stmt *Before, const Stmt *After) const{
        Before = resolveSyntheticStmt(Before);
        After = resolveSyntheticStmt(After);

        // If 'After' is in the subtree of the siblings that follow 'Before' in the
        // chain of successors, we know that 'After' is sequenced after 'Before'.
        for (const Stmt *Successor = getSequenceSuccessor(Before); Successor;
             Successor = getSequenceSuccessor(Successor)) {
            if (isDescendantOrEqual(After, Successor, Context))
                return true;
        }

        // If 'After' is a parent of 'Before' or is sequenced after one of these
        // parents, we know that it is sequenced after 'Before'.
        for (const Stmt *Parent : getParentStmts(Before, Context)) {
            if (Parent == After || inSequence(Parent, After))
                return true;
        }

        return false;
    }

    /// Returns whether \p After can potentially be evaluated after \p Before.
    /// This is exactly equivalent to `!inSequence(After, Before)` but makes some
    /// conditions read more naturally.
    bool potentiallyAfter(const Stmt *After, const Stmt *Before) const{
        return !inSequence(After, Before);
    }

private:
    // 目前parent考虑BinaryOperator，InitListExpr，CompoundStmt，DeclStmt，
    // CXXForRangeStmt，IfStmt，SwitchStmt，WhileStmt这几种情况
    const Stmt *getSequenceSuccessor(const Stmt *S) const{
        for (const Stmt *Parent : getParentStmts(S, Context)) {
            // If a statement has multiple parents, make sure we're using the parent
            // that lies within the sub-tree under Root.
            if (!isDescendantOrEqual(Parent, Root, Context))
                continue;
            if (const auto *BO = dyn_cast<BinaryOperator>(Parent)) {
                // Comma operator: Right-hand side is sequenced after the left-hand side.
                if (BO->getLHS() == S && BO->getOpcode() == BO_Comma)
                    return BO->getRHS();
            } else if (const auto *InitList = dyn_cast<InitListExpr>(Parent)) {
                // Initializer list: Each initializer clause is sequenced after the
                // clauses that precede it.
                for (unsigned I = 1; I < InitList->getNumInits(); ++I) {
                    if (InitList->getInit(I - 1) == S)
                        return InitList->getInit(I);
                }
            } else if (const auto *Compound = dyn_cast<CompoundStmt>(Parent)) {
                // Compound statement: Each sub-statement is sequenced after the
                // statements that precede it.
                const Stmt *Previous = nullptr;
                for (const auto *Child : Compound->body()) {
                    if (Previous == S)
                        return Child;
                    Previous = Child;
                }
            } else if (const auto *TheDeclStmt = dyn_cast<DeclStmt>(Parent)) {
                // Declaration: Every initializer expression is sequenced after the
                // initializer expressions that precede it.
                const Expr *PreviousInit = nullptr;
                for (const Decl *TheDecl : TheDeclStmt->decls()) {
                    if (const auto *TheVarDecl = dyn_cast<VarDecl>(TheDecl)) {
                        if (const Expr *Init = TheVarDecl->getInit()) {
                            if (PreviousInit == S)
                                return Init;
                            PreviousInit = Init;
                        }
                    }
                }
            } else if (const auto *ForRange = dyn_cast<CXXForRangeStmt>(Parent)) {
                // Range-based for: Loop variable declaration is sequenced before the
                // body. (We need this rule because these get placed in the same
                // CFGBlock.)
                if (S == ForRange->getLoopVarStmt())
                    return ForRange->getBody();
            } else if (const auto *TheIfStmt = dyn_cast<IfStmt>(Parent)) {
                // If statement:
                // - Sequence init statement before variable declaration, if present;
                //   before condition evaluation, otherwise.
                // - Sequence variable declaration (along with the expression used to
                //   initialize it) before the evaluation of the condition.
                if (S == TheIfStmt->getInit()) {
                    if (TheIfStmt->getConditionVariableDeclStmt() != nullptr)
                        return TheIfStmt->getConditionVariableDeclStmt();
                    return TheIfStmt->getCond();
                }
                if (S == TheIfStmt->getConditionVariableDeclStmt())
                    return TheIfStmt->getCond();
            } else if (const auto *TheSwitchStmt = dyn_cast<SwitchStmt>(Parent)) {
                // Ditto for switch statements.
                if (S == TheSwitchStmt->getInit()) {
                    if (TheSwitchStmt->getConditionVariableDeclStmt() != nullptr)
                        return TheSwitchStmt->getConditionVariableDeclStmt();
                    return TheSwitchStmt->getCond();
                }
                if (S == TheSwitchStmt->getConditionVariableDeclStmt())
                    return TheSwitchStmt->getCond();
            } else if (const auto *TheWhileStmt = dyn_cast<WhileStmt>(Parent)) {
                // While statement: Sequence variable declaration (along with the
                // expression used to initialize it) before the evaluation of the
                // condition.
                if (S == TheWhileStmt->getConditionVariableDeclStmt())
                    return TheWhileStmt->getCond();
            }
        }

        return nullptr;
    }


    const Stmt *resolveSyntheticStmt(const Stmt *S) const{
        if (SyntheticStmtSourceMap.count(S))
            return SyntheticStmtSourceMap.lookup(S);
        return S;
    }

    ASTContext *Context;
    const Stmt *Root;

    llvm::DenseMap<const Stmt *, const Stmt *> SyntheticStmtSourceMap;
};

/// Maps `Stmt`s to the `CFGBlock` that contains them. Some `Stmt`s may be
/// contained in more than one `CFGBlock`; in this case, they are mapped to the
/// innermost block (i.e. the one that is furthest from the root of the tree).
class StmtToBlockMap {
public:
    /// Initializes the map for the given `CFG`.
    StmtToBlockMap(const CFG *TheCFG, ASTContext *TheContext): Context(TheContext){
        for (const auto *B : *TheCFG) {
            for (const auto &Elem : *B) {
                if (Optional<CFGStmt> S = Elem.getAs<CFGStmt>())
                    Map[S->getStmt()] = B;
            }
        }
    }

    /// Returns the block that \p S is contained in. Some `Stmt`s may be contained
    /// in more than one `CFGBlock`; in this case, this function returns the
    /// innermost block (i.e. the one that is furthest from the root of the tree).
    const CFGBlock *blockContainingStmt(const Stmt *S) const{
        while (!Map.count(S)) {
            SmallVector<const Stmt *, 1> Parents = getParentStmts(S, Context);
            if (Parents.empty())
                return nullptr;
            S = Parents[0];
        }

        return Map.lookup(S);
    }

private:
    ASTContext *Context;

    llvm::DenseMap<const Stmt *, const CFGBlock *> Map;
};