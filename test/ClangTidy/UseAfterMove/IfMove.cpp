//
// Created by prophe on 2022/2/10.
//

#include<iostream>
#include<vector>
#include<string>

void func(){
    std::string str = "Hello, world!\n";
    std::vector<std::string> messages;
    int i;
    std::cin >> i;
    if (i == 1) {
        messages.emplace_back(std::move(str));
    }
    if (i == 2) {
        std::cout << str;
    }
}

/*
AST:
-FunctionDecl 0x2cf8bf8 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/ClangTidy/UseAfterMove/IfMove.cpp:9:1, line:20:1> line:9:6 func 'void ()'
  `-CompoundStmt 0x2d293e0 <col:12, line:20:1>
    |-DeclStmt 0x2cf8f68 <line:10:5, col:40>
    | `-VarDecl 0x2cf8d18 <col:5, col:23> col:17 used str 'std::string':'std::basic_string<char>' cinit destroyed
    |   `-ExprWithCleanups 0x2cf8f50 <col:17, col:23> 'std::string':'std::basic_string<char>'
    |     `-CXXConstructExpr 0x2cf8f20 <col:17, col:23> 'std::string':'std::basic_string<char>' 'void (std::basic_string<char> &&) noexcept' elidable
    |       `-MaterializeTemporaryExpr 0x2cf8f08 <col:23> 'std::string':'std::basic_string<char>' xvalue
    |         `-CXXBindTemporaryExpr 0x2cf8ee8 <col:23> 'std::string':'std::basic_string<char>' (CXXTemporary 0x2cf8ee8)
    |           `-ImplicitCastExpr 0x2cf8ec8 <col:23> 'std::string':'std::basic_string<char>' <ConstructorConversion>
    |             `-CXXConstructExpr 0x2cf8e90 <col:23> 'std::string':'std::basic_string<char>' 'void (const char *, const std::allocator<char> &)'
    |               |-ImplicitCastExpr 0x2cf8da8 <col:23> 'const char *' <ArrayToPointerDecay>
    |               | `-StringLiteral 0x2cf8d80 <col:23> 'const char [15]' lvalue "Hello, world!\n"
    |               `-CXXDefaultArgExpr 0x2cf8e70 <<invalid sloc>> 'const std::allocator<char>':'const std::allocator<char>' lvalue
    |-DeclStmt 0x2d1a768 <line:11:5, col:38>
    | `-VarDecl 0x2cf93a8 <col:5, col:30> col:30 used messages 'std::vector<std::string>':'std::vector<std::basic_string<char>>' callinit destroyed
    |   `-CXXConstructExpr 0x2d1a740 <col:30> 'std::vector<std::string>':'std::vector<std::basic_string<char>>' 'void () noexcept(is_nothrow_default_constructible<allocator<basic_string<char>>>::value)'
    |-DeclStmt 0x2d1a800 <line:12:5, col:10>
    | `-VarDecl 0x2d1a798 <col:5, col:9> col:9 used i 'int'
    |-CXXOperatorCallExpr 0x2d1c080 <line:13:5, col:17> 'std::basic_istream<char>::__istream_type':'std::basic_istream<char>' lvalue '>>'
    | |-ImplicitCastExpr 0x2d1c068 <col:14> 'std::basic_istream<char>::__istream_type &(*)(int &)' <FunctionToPointerDecay>
    | | `-DeclRefExpr 0x2d1bff0 <col:14> 'std::basic_istream<char>::__istream_type &(int &)' lvalue CXXMethod 0x2b9afb8 'operator>>' 'std::basic_istream<char>::__istream_type &(int &)'
    | |-DeclRefExpr 0x2d1a868 <col:5, col:10> 'std::istream':'std::basic_istream<char>' lvalue Var 0x2c16f00 'cin' 'std::istream':'std::basic_istream<char>'
    | `-DeclRefExpr 0x2d1a898 <col:17> 'int' lvalue Var 0x2d1a798 'i' 'int'
    |-IfStmt 0x2d1cf50 <line:14:5, line:16:5>
    | |-BinaryOperator 0x2d1c110 <line:14:9, col:14> 'bool' '=='
    | | |-ImplicitCastExpr 0x2d1c0f8 <col:9> 'int' <LValueToRValue>
    | | | `-DeclRefExpr 0x2d1c0b8 <col:9> 'int' lvalue Var 0x2d1a798 'i' 'int'
    | | `-IntegerLiteral 0x2d1c0d8 <col:14> 'int' 1
    | `-CompoundStmt 0x2d1cf38 <col:17, line:16:5>
    |   `-CXXMemberCallExpr 0x2d1cee0 <line:15:9, col:45> 'void'
    |     |-MemberExpr 0x2d1ce98 <col:9, col:18> '<bound member function type>' .emplace_back 0x2d1cd98
    |     | `-DeclRefExpr 0x2d1c130 <col:9> 'std::vector<std::string>':'std::vector<std::basic_string<char>>' lvalue Var 0x2cf93a8 'messages' 'std::vector<std::string>':'std::vector<std::basic_string<char>>'
    |     `-CallExpr 0x2d1cbe0 <col:31, col:44> 'typename std::remove_reference<basic_string<char> &>::type':'std::basic_string<char>' xvalue
    |       |-ImplicitCastExpr 0x2d1cbc8 <col:31, col:36> 'typename std::remove_reference<basic_string<char> &>::type &&(*)(std::basic_string<char> &) noexcept' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x2d1c998 <col:31, col:36> 'typename std::remove_reference<basic_string<char> &>::type &&(std::basic_string<char> &) noexcept' lvalue Function 0x2d1c898 'move' 'typename std::remove_reference<basic_string<char> &>::type &&(std::basic_string<char> &) noexcept' (FunctionTemplate 0x241ee48 'move')
    |       `-DeclRefExpr 0x2d1c218 <col:41> 'std::string':'std::basic_string<char>' lvalue Var 0x2cf8d18 'str' 'std::string':'std::basic_string<char>'
    `-IfStmt 0x2d293c0 <line:17:5, line:19:5>
      |-BinaryOperator 0x2d1cfc8 <line:17:9, col:14> 'bool' '=='
      | |-ImplicitCastExpr 0x2d1cfb0 <col:9> 'int' <LValueToRValue>
      | | `-DeclRefExpr 0x2d1cf70 <col:9> 'int' lvalue Var 0x2d1a798 'i' 'int'
      | `-IntegerLiteral 0x2d1cf90 <col:14> 'int' 2
      `-CompoundStmt 0x2d293a8 <col:17, line:19:5>
        `-CXXOperatorCallExpr 0x2d29370 <line:18:9, col:22> 'basic_ostream<char>':'std::basic_ostream<char>' lvalue '<<' adl
          |-ImplicitCastExpr 0x2d29358 <col:19> 'basic_ostream<char> &(*)(basic_ostream<char> &, const basic_string<char> &)' <FunctionToPointerDecay>
          | `-DeclRefExpr 0x2d292d8 <col:19> 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)' lvalue Function 0x297f6c8 'operator<<' 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)'
          |-DeclRefExpr 0x2d1d038 <col:9, col:14> 'std::ostream':'std::basic_ostream<char>' lvalue Var 0x2c16f78 'cout' 'std::ostream':'std::basic_ostream<char>'
          `-ImplicitCastExpr 0x2d292c0 <col:22> 'const basic_string<char>':'const std::basic_string<char>' lvalue <NoOp>
            `-DeclRefExpr 0x2d1d068 <col:22> 'std::string':'std::basic_string<char>' lvalue Var 0x2cf8d18 'str' 'std::string':'std::basic_string<char>'
 */


/*
CFG:

 [B6 (ENTRY)]
   Succs (1): B5

 [B1]
   1: [B5.7].~std::vector<std::string>() (Implicit destructor)
   2: [B5.3].~std::string() (Implicit destructor)
   Preds (2): B2 B3
   Succs (1): B0

 [B2]
   1: std::cout << str (OperatorCall)
   Preds (1): B3
   Succs (1): B1

 [B3]
   1: i == 2
   T: if [B3.1]
   Preds (2): B4 B5
   Succs (2): B2 B1

 [B4]
   1: std::move(str)
   2: messages.emplace_back([B4.1])
   Preds (1): B5
   Succs (1): B3

 [B5]
   1: "Hello, world!\n" (CXXConstructExpr, std::string)
   2: [B5.1] (CXXConstructExpr, std::string)
   3: std::string str = "Hello, world!\n";
   4: ~std::string() (Temporary object destructor)
   5: ~std::allocator<char>() (Temporary object destructor)
   6:  (CXXConstructExpr, std::vector<std::string>)
   7: std::vector<std::string> messages;
   8: int i;
   9: std::cin >> i (OperatorCall)
  10: i == 1
   T: if [B5.10]
   Preds (1): B6
   Succs (2): B4 B3

 [B0 (EXIT)]
   Preds (1): B1
 */