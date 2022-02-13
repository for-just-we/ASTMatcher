//
// Created by prophe on 2022/2/1.
//

int foo() {
    int a = 10;

    // expected-warning@+1 {{(Recursive AST Visitor) regular for-loop variable not used}}
    for (int i = 0; i < 10; i++) {
        a++;
        for (int j = 0; j < 10; j++) {
            a+=j;
            // expected-warning@+1 {{(Recursive AST Visitor) regular for-loop variable not used}}
            for (int k = 0; k < 10; k++) {
                a--;
            }
        }
    }

    return a;
}

/*
 -FunctionDecl 0x8a4bde0 <test/UnusedForLoopVar/UnusedForLoopVar-nested.c:5:1, line:21:1> line:5:5 foo 'int ()'
  `-CompoundStmt 0x8a4c5f8 <col:11, line:21:1>
    |-DeclStmt 0x8a4bf68 <line:6:5, col:15>
    | `-VarDecl 0x8a4bee0 <col:5, col:13> col:9 used a 'int' cinit
    |   `-IntegerLiteral 0x8a4bf48 <col:13> 'int' 10
    |-ForStmt 0x8a4c578 <line:9:5, line:18:5>
    | |-DeclStmt 0x8a4c020 <line:9:10, col:19>
    | | `-VarDecl 0x8a4bf98 <col:10, col:18> col:14 used i 'int' cinit
    | |   `-IntegerLiteral 0x8a4c000 <col:18> 'int' 0
    | |-<<<NULL>>>
    | |-BinaryOperator 0x8a4c090 <col:21, col:25> 'int' '<'
    | | |-ImplicitCastExpr 0x8a4c078 <col:21> 'int' <LValueToRValue>
    | | | `-DeclRefExpr 0x8a4c038 <col:21> 'int' lvalue Var 0x8a4bf98 'i' 'int'
    | | `-IntegerLiteral 0x8a4c058 <col:25> 'int' 10
    | |-UnaryOperator 0x8a4c0d0 <col:29, col:30> 'int' postfix '++'
    | | `-DeclRefExpr 0x8a4c0b0 <col:29> 'int' lvalue Var 0x8a4bf98 'i' 'int'
    | `-CompoundStmt 0x8a4c558 <col:34, line:18:5>
    |   |-UnaryOperator 0x8a4c108 <line:10:9, col:10> 'int' postfix '++'
    |   | `-DeclRefExpr 0x8a4c0e8 <col:9> 'int' lvalue Var 0x8a4bee0 'a' 'int'
    |   `-ForStmt 0x8a4c520 <line:11:9, line:17:9>
    |     |-DeclStmt 0x8a4c1c0 <line:11:14, col:23>
    |     | `-VarDecl 0x8a4c138 <col:14, col:22> col:18 used j 'int' cinit
    |     |   `-IntegerLiteral 0x8a4c1a0 <col:22> 'int' 0
    |     |-<<<NULL>>>
    |     |-BinaryOperator 0x8a4c230 <col:25, col:29> 'int' '<'
    |     | |-ImplicitCastExpr 0x8a4c218 <col:25> 'int' <LValueToRValue>
    |     | | `-DeclRefExpr 0x8a4c1d8 <col:25> 'int' lvalue Var 0x8a4c138 'j' 'int'
    |     | `-IntegerLiteral 0x8a4c1f8 <col:29> 'int' 10
    |     |-UnaryOperator 0x8a4c270 <col:33, col:34> 'int' postfix '++'
    |     | `-DeclRefExpr 0x8a4c250 <col:33> 'int' lvalue Var 0x8a4c138 'j' 'int'
    |     `-CompoundStmt 0x8a4c500 <col:38, line:17:9>
    |       |-CompoundAssignOperator 0x8a4c2e0 <line:12:13, col:16> 'int' '+=' ComputeLHSTy='int' ComputeResultTy='int'
    |       | |-DeclRefExpr 0x8a4c288 <col:13> 'int' lvalue Var 0x8a4bee0 'a' 'int'
    |       | `-ImplicitCastExpr 0x8a4c2c8 <col:16> 'int' <LValueToRValue>
    |       |   `-DeclRefExpr 0x8a4c2a8 <col:16> 'int' lvalue Var 0x8a4c138 'j' 'int'
    |       `-ForStmt 0x8a4c4c8 <line:14:13, line:16:13>
    |         |-DeclStmt 0x8a4c3b0 <line:14:18, col:27>
    |         | `-VarDecl 0x8a4c328 <col:18, col:26> col:22 used k 'int' cinit
    |         |   `-IntegerLiteral 0x8a4c390 <col:26> 'int' 0
    |         |-<<<NULL>>>
    |         |-BinaryOperator 0x8a4c420 <col:29, col:33> 'int' '<'
    |         | |-ImplicitCastExpr 0x8a4c408 <col:29> 'int' <LValueToRValue>
    |         | | `-DeclRefExpr 0x8a4c3c8 <col:29> 'int' lvalue Var 0x8a4c328 'k' 'int'
    |         | `-IntegerLiteral 0x8a4c3e8 <col:33> 'int' 10
    |         |-UnaryOperator 0x8a4c460 <col:37, col:38> 'int' postfix '++'
    |         | `-DeclRefExpr 0x8a4c440 <col:37> 'int' lvalue Var 0x8a4c328 'k' 'int'
    |         `-CompoundStmt 0x8a4c4b0 <col:42, line:16:13>
    |           `-UnaryOperator 0x8a4c498 <line:15:17, col:18> 'int' postfix '--'
    |             `-DeclRefExpr 0x8a4c478 <col:17> 'int' lvalue Var 0x8a4bee0 'a' 'int'
    `-ReturnStmt 0x8a4c5e8 <line:20:5, col:12>
      `-ImplicitCastExpr 0x8a4c5d0 <col:12> 'int' <LValueToRValue>
        `-DeclRefExpr 0x8a4c5b0 <col:12> 'int' lvalue Var 0x8a4bee0 'a' 'int'
 */