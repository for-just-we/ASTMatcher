//
// Created by prophe on 2022/2/11.
//

#include<iostream>
#include<vector>
#include<string>

void func(){
    std::string str = "Hello, world!\n"; // Move Variable 1
    std::vector<std::string> messages;
    int i;
    std::cin >> i;
    if (i == 1) {
        messages.emplace_back(std::move(str)); // MovingCall 1
        str = "";
    }

    if (i == 2) {
        std::cout << str;
    }

    std::string str1 = "damn it!\n"; // MoveVariable 2
    if (i == 3){
        std::move(str1); // MovingCall 2
    }
    std::cout << str1;
}

/*
AST:
CompoundStmt 0x3a56d60
|-DeclStmt 0x3a254a8
| `-VarDecl 0x3a25258  used str 'std::string':'class std::basic_string<char>' cinit destroyed
|   `-ExprWithCleanups 0x3a25490 'std::string':'class std::basic_string<char>'
|     `-CXXConstructExpr 0x3a25460 'std::string':'class std::basic_string<char>' 'void (class std::basic_string<char> &&) noexcept' elidable
|       `-MaterializeTemporaryExpr 0x3a25448 'std::string':'class std::basic_string<char>' xvalue
|         `-CXXBindTemporaryExpr 0x3a25428 'std::string':'class std::basic_string<char>' (CXXTemporary 0x3a25428)
|           `-ImplicitCastExpr 0x3a25408 'std::string':'class std::basic_string<char>' <ConstructorConversion>
|             `-CXXConstructExpr 0x3a253d0 'std::string':'class std::basic_string<char>' 'void (const char *, const class std::allocator<char> &)'
|               |-ImplicitCastExpr 0x3a252e8 'const char *' <ArrayToPointerDecay>
|               | `-StringLiteral 0x3a252c0 'const char [15]' lvalue "Hello, world!\n"
|               `-CXXDefaultArgExpr 0x3a253b0 'const class std::allocator<char>':'const class std::allocator<char>' lvalue
|-DeclStmt 0x3a46ca8
| `-VarDecl 0x3a258e8  used messages 'std::vector<std::string>':'class std::vector<class std::basic_string<char> >' callinit destroyed
|   `-CXXConstructExpr 0x3a46c80 'std::vector<std::string>':'class std::vector<class std::basic_string<char> >' 'void (void) noexcept(is_nothrow_default_constructible<class allocator<class basic_string<char> > >::value)'
|-DeclStmt 0x3a46d40
| `-VarDecl 0x3a46cd8  used i 'int'
|-CXXOperatorCallExpr 0x3a485c0 'std::basic_istream<char>::__istream_type':'class std::basic_istream<char>' lvalue '>>'
| |-ImplicitCastExpr 0x3a485a8 'std::basic_istream<char>::__istream_type &(*)(int &)' <FunctionToPointerDecay>
| | `-DeclRefExpr 0x3a48530 'std::basic_istream<char>::__istream_type &(int &)' lvalue CXXMethod 0x38c7e48 'operator>>' 'std::basic_istream<char>::__istream_type &(int &)'
| |-DeclRefExpr 0x3a46da8 'std::istream':'class std::basic_istream<char>' lvalue Var 0x3945b90 'cin' 'std::istream':'class std::basic_istream<char>'
| `-DeclRefExpr 0x3a46dd8 'int' lvalue Var 0x3a46cd8 'i' 'int'
|-IfStmt 0x3a49768
| |-BinaryOperator 0x3a48650 '_Bool' '=='
| | |-ImplicitCastExpr 0x3a48638 'int' <LValueToRValue>
| | | `-DeclRefExpr 0x3a485f8 'int' lvalue Var 0x3a46cd8 'i' 'int'
| | `-IntegerLiteral 0x3a48618 'int' 1
| `-CompoundStmt 0x3a49748
|   |-CXXMemberCallExpr 0x3a49420 'void'
|   | |-MemberExpr 0x3a493d8 '<bound member function type>' .emplace_back 0x3a492d8
|   | | `-DeclRefExpr 0x3a48670 'std::vector<std::string>':'class std::vector<class std::basic_string<char> >' lvalue Var 0x3a258e8 'messages' 'std::vector<std::string>':'class std::vector<class std::basic_string<char> >'
|   | `-CallExpr 0x3a49120 'typename std::remove_reference<class basic_string<char> &>::type':'class std::basic_string<char>' xvalue
|   |   |-ImplicitCastExpr 0x3a49108 'typename std::remove_reference<class basic_string<char> &>::type &&(*)(class std::basic_string<char> &) noexcept' <FunctionToPointerDecay>
|   |   | `-DeclRefExpr 0x3a48ed8 'typename std::remove_reference<class basic_string<char> &>::type &&(class std::basic_string<char> &) noexcept' lvalue Function 0x3a48dd8 'move' 'typename std::remove_reference<class basic_string<char> &>::type &&(class std::basic_string<char> &) noexcept' (FunctionTemplate 0x314c2f8 'move')
|   |   `-DeclRefExpr 0x3a48758 'std::string':'class std::basic_string<char>' lvalue Var 0x3a25258 'str' 'std::string':'class std::basic_string<char>'
|   `-CXXOperatorCallExpr 0x3a49710 'class std::basic_string<char>' lvalue '='
|     |-ImplicitCastExpr 0x3a496f8 'class std::basic_string<char> &(*)(const char *)' <FunctionToPointerDecay>
|     | `-DeclRefExpr 0x3a49680 'class std::basic_string<char> &(const char *)' lvalue CXXMethod 0x358a8d8 'operator=' 'class std::basic_string<char> &(const char *)'
|     |-DeclRefExpr 0x3a49478 'std::string':'class std::basic_string<char>' lvalue Var 0x3a25258 'str' 'std::string':'class std::basic_string<char>'
|     `-ImplicitCastExpr 0x3a49668 'const char *' <ArrayToPointerDecay>
|       `-StringLiteral 0x3a494d8 'const char [1]' lvalue ""
|-IfStmt 0x3a55be0
| |-BinaryOperator 0x3a497e0 '_Bool' '=='
| | |-ImplicitCastExpr 0x3a497c8 'int' <LValueToRValue>
| | | `-DeclRefExpr 0x3a49788 'int' lvalue Var 0x3a46cd8 'i' 'int'
| | `-IntegerLiteral 0x3a497a8 'int' 2
| `-CompoundStmt 0x3a55bc8
|   `-CXXOperatorCallExpr 0x3a55b90 'basic_ostream<char>':'class std::basic_ostream<char>' lvalue '<<' adl
|     |-ImplicitCastExpr 0x3a55b78 'basic_ostream<char> &(*)(basic_ostream<char> &, const basic_string<char> &)' <FunctionToPointerDecay>
|     | `-DeclRefExpr 0x3a55af8 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)' lvalue Function 0x36ac8a8 'operator<<' 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)'
|     |-DeclRefExpr 0x3a49850 'std::ostream':'class std::basic_ostream<char>' lvalue Var 0x3945c08 'cout' 'std::ostream':'class std::basic_ostream<char>'
|     `-ImplicitCastExpr 0x3a55ae0 'const basic_string<char>':'const class std::basic_string<char>' lvalue <NoOp>
|       `-DeclRefExpr 0x3a49880 'std::string':'class std::basic_string<char>' lvalue Var 0x3a25258 'str' 'std::string':'class std::basic_string<char>'
|-DeclStmt 0x3a55e60
| `-VarDecl 0x3a55c48  used str1 'std::string':'class std::basic_string<char>' cinit destroyed
|   `-ExprWithCleanups 0x3a55e48 'std::string':'class std::basic_string<char>'
|     `-CXXConstructExpr 0x3a55e18 'std::string':'class std::basic_string<char>' 'void (class std::basic_string<char> &&) noexcept' elidable
|       `-MaterializeTemporaryExpr 0x3a55e00 'std::string':'class std::basic_string<char>' xvalue
|         `-CXXBindTemporaryExpr 0x3a55de0 'std::string':'class std::basic_string<char>' (CXXTemporary 0x3a55de0)
|           `-ImplicitCastExpr 0x3a55dc0 'std::string':'class std::basic_string<char>' <ConstructorConversion>
|             `-CXXConstructExpr 0x3a55d88 'std::string':'class std::basic_string<char>' 'void (const char *, const class std::allocator<char> &)'
|               |-ImplicitCastExpr 0x3a55d50 'const char *' <ArrayToPointerDecay>
|               | `-StringLiteral 0x3a55d28 'const char [10]' lvalue "damn it!\n"
|               `-CXXDefaultArgExpr 0x3a55d68 'const class std::allocator<char>':'const class std::allocator<char>' lvalue
|-IfStmt 0x3a56048
| |-BinaryOperator 0x3a55ed0 '_Bool' '=='
| | |-ImplicitCastExpr 0x3a55eb8 'int' <LValueToRValue>
| | | `-DeclRefExpr 0x3a55e78 'int' lvalue Var 0x3a46cd8 'i' 'int'
| | `-IntegerLiteral 0x3a55e98 'int' 3
| `-CompoundStmt 0x3a56030
|   `-CallExpr 0x3a56008 'typename std::remove_reference<class basic_string<char> &>::type':'class std::basic_string<char>' xvalue
|     |-ImplicitCastExpr 0x3a55ff0 'typename std::remove_reference<class basic_string<char> &>::type &&(*)(class std::basic_string<char> &) noexcept' <FunctionToPointerDecay>
|     | `-DeclRefExpr 0x3a55fb8 'typename std::remove_reference<class basic_string<char> &>::type &&(class std::basic_string<char> &) noexcept' lvalue Function 0x3a48dd8 'move' 'typename std::remove_reference<class basic_string<char> &>::type &&(class std::basic_string<char> &) noexcept' (FunctionTemplate 0x314c2f8 'move')
|     `-DeclRefExpr 0x3a55f60 'std::string':'class std::basic_string<char>' lvalue Var 0x3a55c48 'str1' 'std::string':'class std::basic_string<char>'
`-CXXOperatorCallExpr 0x3a56d28 'basic_ostream<char>':'class std::basic_ostream<char>' lvalue '<<' adl
  |-ImplicitCastExpr 0x3a56d10 'basic_ostream<char> &(*)(basic_ostream<char> &, const basic_string<char> &)' <FunctionToPointerDecay>
  | `-DeclRefExpr 0x3a56cf0 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)' lvalue Function 0x36ac8a8 'operator<<' 'basic_ostream<char> &(basic_ostream<char> &, const basic_string<char> &)'
  |-DeclRefExpr 0x3a560b8 'std::ostream':'class std::basic_ostream<char>' lvalue Var 0x3945c08 'cout' 'std::ostream':'class std::basic_ostream<char>'
  `-ImplicitCastExpr 0x3a56cd8 'const basic_string<char>':'const class std::basic_string<char>' lvalue <NoOp>
    `-DeclRefExpr 0x3a560e8 'std::string':'class std::basic_string<char>' lvalue Var 0x3a55c48 'str1' 'std::string':'class std::basic_string<char>'
*/


/*
CFG:
[B8 (ENTRY)]
   Succs (1): B7

 [B1]
   1: std::cout << str1 (OperatorCall)
   2: [B3.3].~std::string() (Implicit destructor)
   3: [B7.7].~std::vector<std::string>() (Implicit destructor)
   4: [B7.3].~std::string() (Implicit destructor)
   Preds (2): B2 B3
   Succs (1): B0

 [B2]
   1: std::move(str1)
   Preds (1): B3
   Succs (1): B1

 [B3]
   1: "damn it!\n" (CXXConstructExpr, std::string)
   2: [B3.1] (CXXConstructExpr, std::string)
   3: std::string str1 = "damn it!\n";
   4: ~std::string() (Temporary object destructor)
   5: ~std::allocator<char>() (Temporary object destructor)
   6: i == 3
   T: if [B3.6]
   Preds (2): B4 B5
   Succs (2): B2 B1

 [B4]
   1: std::cout << str (OperatorCall)
   Preds (1): B5
   Succs (1): B3

 [B5]
   1: i == 2
   T: if [B5.1]
   Preds (2): B6 B7
   Succs (2): B4 B3

 [B6]
   1: std::move(str) # use
   2: messages.emplace_back([B6.1]) # use
   3: str = "" (OperatorCall) # reinit, use
   Preds (1): B7
   Succs (1): B5

 [B7]
   1: "Hello, world!\n" (CXXConstructExpr, std::string)
   2: [B7.1] (CXXConstructExpr, std::string)
   3: std::string str = "Hello, world!\n";
   4: ~std::string() (Temporary object destructor)
   5: ~std::allocator<char>() (Temporary object destructor)
   6:  (CXXConstructExpr, std::vector<std::string>)
   7: std::vector<std::string> messages;
   8: int i;
   9: std::cin >> i (OperatorCall)
  10: i == 1
   T: if [B7.10]
   Preds (1): B8
   Succs (2): B6 B5

 [B0 (EXIT)]
   Preds (1): B1
 */