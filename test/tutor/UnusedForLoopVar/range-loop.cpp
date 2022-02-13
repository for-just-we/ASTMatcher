//
// Created by prophe on 2022/2/1.
#include <vector>

int bar(int var_a) {
    std::vector<int> var_b = {1, 2, 3};

    // expected-warning@+1 {{(AST Matcher) range for-loop variable not used}}
    for (auto some_integer: var_b)
    var_a++;

    return var_a;
}

/*
 -FunctionDecl 0x8a97258 <test/UnusedForLoopVar/UnusedForLoopVar-range-loop.c:5:1, line:13:1> line:5:5 bar 'int (int)'
  |-ParmVarDecl 0x8a971c0 <col:9, col:13> col:13 used var_a 'int'
  `-CompoundStmt 0x8abed00 <col:20, line:13:1>
    |-DeclStmt 0x8ab8b70 <line:6:5, col:39>
    | `-VarDecl 0x8a976f8 <col:5, col:38> col:22 used var_b 'std::vector<int>':'std::vector<int>' cinit destroyed
    |   `-ExprWithCleanups 0x8ab8b58 <col:30, col:38> 'std::vector<int>':'std::vector<int>'
    |     `-CXXConstructExpr 0x8ab8b20 <col:30, col:38> 'std::vector<int>':'std::vector<int>' 'void (initializer_list<std::vector<int>::value_type>, const std::vector<int>::allocator_type &)' list std::initializer_list
    |       |-CXXStdInitializerListExpr 0x8ab8950 <col:30, col:38> 'initializer_list<std::vector<int>::value_type>':'std::initializer_list<int>'
    |       | `-MaterializeTemporaryExpr 0x8ab8938 <col:30, col:38> 'const int [3]' xvalue
    |       |   `-InitListExpr 0x8ab7848 <col:30, col:38> 'const int [3]'
    |       |     |-IntegerLiteral 0x8a97760 <col:31> 'int' 1
    |       |     |-IntegerLiteral 0x8a97780 <col:34> 'int' 2
    |       |     `-IntegerLiteral 0x8a977a0 <col:37> 'int' 3
    |       `-CXXDefaultArgExpr 0x8ab8b00 <<invalid sloc>> 'const std::vector<int>::allocator_type':'const std::allocator<int>' lvalue
    |-CXXForRangeStmt 0x8abec20 <line:9:5, line:10:10>
    | |-<<<NULL>>>
    | |-DeclStmt 0x8ab8ed0 <line:9:29>
    | | `-VarDecl 0x8ab8cc8 <col:29> col:29 implicit used __range1 'std::vector<int> &' cinit
    | |   `-DeclRefExpr 0x8ab8b88 <col:29> 'std::vector<int>':'std::vector<int>' lvalue Var 0x8a976f8 'var_b' 'std::vector<int>':'std::vector<int>'
    | |-DeclStmt 0x8abdb98 <col:27>
    | | `-VarDecl 0x8ab8f60 <col:27> col:27 implicit used __begin1 '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' cinit
    | |   `-ExprWithCleanups 0x8abd9b0 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |     `-CXXConstructExpr 0x8abd980 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' 'void (__gnu_cxx::__normal_iterator<int *, std::vector<int>> &&) noexcept' elidable
    | |       `-MaterializeTemporaryExpr 0x8abd838 <col:27> 'std::vector<int>::iterator':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' xvalue
    | |         `-CXXMemberCallExpr 0x8ab90f8 <col:27> 'std::vector<int>::iterator':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |           `-MemberExpr 0x8ab90c8 <col:27> '<bound member function type>' .begin 0x8aab310
    | |             `-DeclRefExpr 0x8ab8ee8 <col:27> 'std::vector<int>' lvalue Var 0x8ab8cc8 '__range1' 'std::vector<int> &'
    | |-DeclStmt 0x8abdbb0 <col:27>
    | | `-VarDecl 0x8ab9000 <col:27> col:27 implicit used __end1 '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' cinit
    | |   `-ExprWithCleanups 0x8abdb80 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |     `-CXXConstructExpr 0x8abdb50 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' 'void (__gnu_cxx::__normal_iterator<int *, std::vector<int>> &&) noexcept' elidable
    | |       `-MaterializeTemporaryExpr 0x8abdb38 <col:27> 'std::vector<int>::iterator':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' xvalue
    | |         `-CXXMemberCallExpr 0x8abda58 <col:27> 'std::vector<int>::iterator':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |           `-MemberExpr 0x8abda28 <col:27> '<bound member function type>' .end 0x8aab518
    | |             `-DeclRefExpr 0x8ab8f08 <col:27> 'std::vector<int>' lvalue Var 0x8ab8cc8 '__range1' 'std::vector<int> &'
    | |-CXXOperatorCallExpr 0x8abe8e0 <col:27> 'bool' '!=' adl
    | | |-ImplicitCastExpr 0x8abe8c8 <col:27> 'bool (*)(const __normal_iterator<int *, std::vector<int>> &, const __normal_iterator<int *, std::vector<int>> &) noexcept' <FunctionToPointerDecay>
    | | | `-DeclRefExpr 0x8abe850 <col:27> 'bool (const __normal_iterator<int *, std::vector<int>> &, const __normal_iterator<int *, std::vector<int>> &) noexcept' lvalue Function 0x8abe550 'operator!=' 'bool (const __normal_iterator<int *, std::vector<int>> &, const __normal_iterator<int *, std::vector<int>> &) noexcept'
    | | |-ImplicitCastExpr 0x8abe820 <col:27> 'const __normal_iterator<int *, std::vector<int>>':'const __gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue <NoOp>
    | | | `-DeclRefExpr 0x8abdbc8 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue Var 0x8ab8f60 '__begin1' '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | | `-ImplicitCastExpr 0x8abe838 <col:27> 'const __normal_iterator<int *, std::vector<int>>':'const __gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue <NoOp>
    | |   `-DeclRefExpr 0x8abdbe8 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue Var 0x8ab9000 '__end1' '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |-CXXOperatorCallExpr 0x8abea00 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue '++'
    | | |-ImplicitCastExpr 0x8abe9e8 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>> &(*)() noexcept' <FunctionToPointerDecay>
    | | | `-DeclRefExpr 0x8abe968 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>> &() noexcept' lvalue CXXMethod 0x8abadb0 'operator++' '__gnu_cxx::__normal_iterator<int *, std::vector<int>> &() noexcept'
    | | `-DeclRefExpr 0x8abe918 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue Var 0x8ab8f60 '__begin1' '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | |-DeclStmt 0x8ab8c48 <col:10, col:34>
    | | `-VarDecl 0x8ab8be0 <col:10, col:27> col:15 some_integer 'int':'int' cinit
    | |   `-ImplicitCastExpr 0x8abec08 <col:27> 'int':'int' <LValueToRValue>
    | |     `-CXXOperatorCallExpr 0x8abeb20 <col:27> 'int':'int' lvalue '*'
    | |       |-ImplicitCastExpr 0x8abeb08 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>::reference (*)() const noexcept' <FunctionToPointerDecay>
    | |       | `-DeclRefExpr 0x8abea90 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>::reference () const noexcept' lvalue CXXMethod 0x8abab00 'operator*' '__gnu_cxx::__normal_iterator<int *, std::vector<int>>::reference () const noexcept'
    | |       `-ImplicitCastExpr 0x8abea78 <col:27> 'const __gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue <NoOp>
    | |         `-DeclRefExpr 0x8abea30 <col:27> '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>' lvalue Var 0x8ab8f60 '__begin1' '__gnu_cxx::__normal_iterator<int *, std::vector<int>>':'__gnu_cxx::__normal_iterator<int *, std::vector<int>>'
    | `-UnaryOperator 0x8abeca0 <line:10:5, col:10> 'int' postfix '++'
    |   `-DeclRefExpr 0x8abec80 <col:5> 'int' lvalue ParmVar 0x8a971c0 'var_a' 'int'
    `-ReturnStmt 0x8abecf0 <line:12:5, col:12>
      `-ImplicitCastExpr 0x8abecd8 <col:12> 'int' <LValueToRValue>
        `-DeclRefExpr 0x8abecb8 <col:12> 'int' lvalue ParmVar 0x8a971c0 'var_a' 'int'

 */