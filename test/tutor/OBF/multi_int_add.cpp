//
// Created by prophe on 2022/2/6.
//

int foo(int a, int b) {
    return a + b + 123;
}

int foo1(int a, int b){
    return a + b * (a + b);
}

/*
TranslationUnitDecl 0x75ad6f8 <<invalid sloc>> <invalid sloc>
`-FunctionDecl 0x75ee9c0 <multi_int_add.cpp:5:1, line:7:1> line:5:5 foo 'int (int, int)'
  |-ParmVarDecl 0x75ee868 <col:9, col:13> col:13 used a 'int'
  |-ParmVarDecl 0x75ee8e8 <col:16, col:20> col:20 used b 'int'
  `-CompoundStmt 0x75eeb98 <col:23, line:7:1>
    `-ReturnStmt 0x75eeb88 <line:6:5, col:20>
      `-BinaryOperator 0x75eeb68 <col:12, col:20> 'int' '+'
        |-BinaryOperator 0x75eeb28 <col:12, col:16> 'int' '+'
        | |-ImplicitCastExpr 0x75eeaf8 <col:12> 'int' <LValueToRValue>
        | | `-DeclRefExpr 0x75eeab8 <col:12> 'int' lvalue ParmVar 0x75ee868 'a' 'int'
        | `-ImplicitCastExpr 0x75eeb10 <col:16> 'int' <LValueToRValue>
        |   `-DeclRefExpr 0x75eead8 <col:16> 'int' lvalue ParmVar 0x75ee8e8 'b' 'int'
        `-IntegerLiteral 0x75eeb48 <col:20> 'int' 123

 */