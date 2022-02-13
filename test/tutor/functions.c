//
// Created by prophe on 2022/1/31.
//

/*
 |-FunctionDecl 0x8e92b38 <test/functions.c:5:1, col:10> col:6 foo 'void ()'
` -FunctionDecl 0x8e92d00 <line:7:1, line:9:1> line:7:5 bar 'int (int)'
  |-ParmVarDecl 0x8e92c38 <col:9, col:13> col:13 used baz 'int'
  `-CompoundStmt 0x8e92df0 <col:18, line:9:1>
    `-ReturnStmt 0x8e92de0 <line:8:5, col:12>
      `-ImplicitCastExpr 0x8e92dc8 <col:12> 'int' <LValueToRValue>
        `-DeclRefExpr 0x8e92da8 <col:12> 'int' lvalue ParmVar 0x8e92c38 'baz' 'int'
 */

void foo();

int bar(int baz) {
    return baz;
}