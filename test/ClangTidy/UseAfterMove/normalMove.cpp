//
// Created by prophe on 2022/2/7.
//

#include<iostream>
#include<vector>
#include<string>

void func(){
    std::string str = "Hello, world!\n";
    std::vector<std::string> messages;
    messages.emplace_back(std::move(str));
    std::cout << str;
}

/*
 * arg: str
 * call-move: std::move(str)
 * containing-func: void func(){
 * moving-call:messages.emplace_back(std::move(str))
 */

/*AST
`-FunctionDecl 0x940e718 <normalMove.cpp:9:1, line:14:1> line:9:6 func 'void ()'
  `-CompoundStmt 0x943d488 <col:12, line:14:1>
    |-DeclStmt 0x940ea88 <line:10:5, col:40>
    | `-VarDecl 0x940e838 <col:5, col:23> col:17 used str 'std::string':'std::basic_string<char>' cinit destroyed
    |   `-ExprWithCleanups 0x940ea70 <col:17, col:23> 'std::string':'std::basic_string<char>'
    |     `-CXXConstructExpr 0x940ea40 <col:17, col:23> 'std::string':'std::basic_string<char>' 'void (std::basic_string<char> &&) noexcept' elidable
    |       `-MaterializeTemporaryExpr 0x940ea28 <col:23> 'std::string':'std::basic_string<char>' xvalue
    |         `-CXXBindTemporaryExpr 0x940ea08 <col:23> 'std::string':'std::basic_string<char>' (CXXTemporary 0x940ea08)
    |           `-ImplicitCastExpr 0x940e9e8 <col:23> 'std::string':'std::basic_string<char>' <ConstructorConversion>
    |             `-CXXConstructExpr 0x940e9b0 <col:23> 'std::string':'std::basic_string<char>' 'void (const char *, const std::allocator<char> &)'
    |               |-ImplicitCastExpr 0x940e8c8 <col:23> 'const char *' <ArrayToPointerDecay>
    |               | `-StringLiteral 0x940e8a0 <col:23> 'const char [15]' lvalue "Hello, world!\n"
    |               `-CXXDefaultArgExpr 0x940e990 <<invalid sloc>> 'const std::allocator<char>':'const std::allocator<char>' lvalue
    |-DeclStmt 0x9430288 <line:11:5, col:38>
    | `-VarDecl 0x940eec8 <col:5, col:30> col:30 used messages 'std::vector<std::string>':'std::vector<std::basic_string<char>>' callinit destroyed
    |   `-CXXConstructExpr 0x9430260 <col:30> 'std::vector<std::string>':'std::vector<std::basic_string<char>>' 'void () noexcept(is_nothrow_default_constructible<allocator<basic_string<char>>>::value)'
    |-CXXMemberCallExpr 0x9431050 <line:12:5, col:41> 'void'
    | |-MemberExpr 0x9431008 <col:5, col:14> '<bound member function type>' .emplace_back 0x9430f08
    | | `-DeclRefExpr 0x94302a0 <col:5> 'std::vector<std::string>':'std::vector<std::basic_string<char>>' lvalue Var 0x940eec8 'messages' 'std::vector<std::string>':'std::vector<std::basic_string<char>>'
    | `-CallExpr 0x9430d50 <col:27, col:40> 'typename std::remove_reference<basic_string<char> &>::type':'std::basic_string<char>' xvalue
    |   |-ImplicitCastExpr 0x9430d38 <col:27, col:32> 'typename std::remove_reference<basic_string<char> &>::type &&(*)(std::basic_string<char> &) noexcept' <FunctionToPointerDecay>
    |   | `-DeclRefExpr 0x9430b08 <col:27, col:32> 'typename std::remove_reference<basic_string<char> &>::type &&(std::basic_string<char> &) noexcept' lvalue Function 0x9430a08 'move' 'typename std::remove_reference<basic_string<char> &>::type &&(std::basic_string<char> &) noexcept' (FunctionTemplate 0x8b35688 'move')
    |   `-DeclRefExpr 0x9430388 <col:37> 'std::string':'std::basic_string<char>' lvalue Var 0x940e838 'str' 'std::string':'std::basic_string<char>'
    `-CXXOperatorCallExpr 0x943d450 <line:13:5, col:18> 'basic_ostream<char>':'std::basic_ostream<char>' lvalue '<<' adl
      |-ImplicitCastExpr 0x943d438 <col:15> 'basic_ostream<char> &(*)(basic_ostream<char> &, const basic_string<char> &)' <FunctionToPointerDecay>
      | `-DeclRefExpr 0x943d3b8 <col:15> 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)' lvalue Function 0x9094438 'operator<<' 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)'
      |-DeclRefExpr 0x94310f8 <col:5, col:10> 'std::ostream':'std::basic_ostream<char>' lvalue Var 0x932c2b8 'cout' 'std::ostream':'std::basic_ostream<char>'
      `-ImplicitCastExpr 0x943d3a0 <col:18> 'const basic_string<char>':'const std::basic_string<char>' lvalue <NoOp>
        `-DeclRefExpr 0x9431128 <col:18> 'std::string':'std::basic_string<char>' lvalue Var 0x940e838 'str' 'std::string':'std::basic_string<char>'
 */