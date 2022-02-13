//
// Created by prophe on 2022/2/6.
//

int foo(int a, int b) {
    return a + b;
}

short bar(short a, short b) {
    return a + b;
}

long bez(long a, long b) {
    return a + b;
}

int fez(long a, short b) {
    return a + b;
}

int fer(int a) {
    return a + 123;
}

int ber() {
    return 321 + 123;
}

/*
TranslationUnitDecl 0x8d07838 <<invalid sloc>> <invalid sloc>
|-FunctionDecl 0x8d48a80 <int_add.cpp:5:1, line:7:1> line:5:5 foo 'int (int, int)'
| |-ParmVarDecl 0x8d48928 <col:9, col:13> col:13 used a 'int'
| |-ParmVarDecl 0x8d489a8 <col:16, col:20> col:20 used b 'int'
| `-CompoundStmt 0x8d48c18 <col:23, line:7:1>
|   `-ReturnStmt 0x8d48c08 <line:6:5, col:16>
|     `-BinaryOperator 0x8d48be8 <col:12, col:16> 'int' '+'
|       |-ImplicitCastExpr 0x8d48bb8 <col:12> 'int' <LValueToRValue>
|       | `-DeclRefExpr 0x8d48b78 <col:12> 'int' lvalue ParmVar 0x8d48928 'a' 'int'
|       `-ImplicitCastExpr 0x8d48bd0 <col:16> 'int' <LValueToRValue>
|         `-DeclRefExpr 0x8d48b98 <col:16> 'int' lvalue ParmVar 0x8d489a8 'b' 'int'
|-FunctionDecl 0x8d48da0 <line:9:1, line:11:1> line:9:7 bar 'short (short, short)'
| |-ParmVarDecl 0x8d48c48 <col:11, col:17> col:17 used a 'short'
| |-ParmVarDecl 0x8d48cc8 <col:20, col:26> col:26 used b 'short'
| `-CompoundStmt 0x8d48f38 <col:29, line:11:1>
|   `-ReturnStmt 0x8d48f28 <line:10:5, col:16>
|     `-ImplicitCastExpr 0x8d48f10 <col:12, col:16> 'short' <IntegralCast>
|       `-BinaryOperator 0x8d48ef0 <col:12, col:16> 'int' '+'
|         |-ImplicitCastExpr 0x8d48ea8 <col:12> 'int' <IntegralCast>
|         | `-ImplicitCastExpr 0x8d48e90 <col:12> 'short' <LValueToRValue>
|         |   `-DeclRefExpr 0x8d48e50 <col:12> 'short' lvalue ParmVar 0x8d48c48 'a' 'short'
|         `-ImplicitCastExpr 0x8d48ed8 <col:16> 'int' <IntegralCast>
|           `-ImplicitCastExpr 0x8d48ec0 <col:16> 'short' <LValueToRValue>
|             `-DeclRefExpr 0x8d48e70 <col:16> 'short' lvalue ParmVar 0x8d48cc8 'b' 'short'
|-FunctionDecl 0x8d490c0 <line:13:1, line:15:1> line:13:6 bez 'long (long, long)'
| |-ParmVarDecl 0x8d48f68 <col:10, col:15> col:15 used a 'long'
| |-ParmVarDecl 0x8d48fe8 <col:18, col:23> col:23 used b 'long'
| `-CompoundStmt 0x8d49210 <col:26, line:15:1>
|   `-ReturnStmt 0x8d49200 <line:14:5, col:16>
|     `-BinaryOperator 0x8d491e0 <col:12, col:16> 'long' '+'
|       |-ImplicitCastExpr 0x8d491b0 <col:12> 'long' <LValueToRValue>
|       | `-DeclRefExpr 0x8d49170 <col:12> 'long' lvalue ParmVar 0x8d48f68 'a' 'long'
|       `-ImplicitCastExpr 0x8d491c8 <col:16> 'long' <LValueToRValue>
|         `-DeclRefExpr 0x8d49190 <col:16> 'long' lvalue ParmVar 0x8d48fe8 'b' 'long'
|-FunctionDecl 0x8d493a0 <line:17:1, line:19:1> line:17:5 fez 'int (long, short)'
| |-ParmVarDecl 0x8d49240 <col:9, col:14> col:14 used a 'long'
| |-ParmVarDecl 0x8d492c0 <col:17, col:23> col:23 used b 'short'
| `-CompoundStmt 0x8d49520 <col:26, line:19:1>
|   `-ReturnStmt 0x8d49510 <line:18:5, col:16>
|     `-ImplicitCastExpr 0x8d494f8 <col:12, col:16> 'int' <IntegralCast>
|       `-BinaryOperator 0x8d494d8 <col:12, col:16> 'long' '+'
|         |-ImplicitCastExpr 0x8d49490 <col:12> 'long' <LValueToRValue>
|         | `-DeclRefExpr 0x8d49450 <col:12> 'long' lvalue ParmVar 0x8d49240 'a' 'long'
|         `-ImplicitCastExpr 0x8d494c0 <col:16> 'long' <IntegralCast>
|           `-ImplicitCastExpr 0x8d494a8 <col:16> 'short' <LValueToRValue>
|             `-DeclRefExpr 0x8d49470 <col:16> 'short' lvalue ParmVar 0x8d492c0 'b' 'short'
|-FunctionDecl 0x8d49620 <line:21:1, line:23:1> line:21:5 fer 'int (int)'
| |-ParmVarDecl 0x8d49550 <col:9, col:13> col:13 used a 'int'
| `-CompoundStmt 0x8d49750 <col:16, line:23:1>
|   `-ReturnStmt 0x8d49740 <line:22:5, col:16>
|     `-BinaryOperator 0x8d49720 <col:12, col:16> 'int' '+'
|       |-ImplicitCastExpr 0x8d49708 <col:12> 'int' <LValueToRValue>
|       | `-DeclRefExpr 0x8d496c8 <col:12> 'int' lvalue ParmVar 0x8d49550 'a' 'int'
|       `-IntegerLiteral 0x8d496e8 <col:16> 'int' 123
`-FunctionDecl 0x8d729a0 <line:25:1, line:27:1> line:25:5 ber 'int ()'
  `-CompoundStmt 0x8d72ab0 <col:11, line:27:1>
    `-ReturnStmt 0x8d72aa0 <line:26:5, col:18>
      `-BinaryOperator 0x8d72a80 <col:12, col:18> 'int' '+'
        |-IntegerLiteral 0x8d72a40 <col:12> 'int' 321
        `-IntegerLiteral 0x8d72a60 <col:18> 'int' 123

binaryOperator(hasOperatorName("+"),hasLHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("lhs"),integerLiteral().bind("lhs"))),hasRHS(anyOf(implicitCastExpr(hasType(isSignedInteger())).bind("rhs"),integerLiteral().bind("rhs")))).bind("op");
 */