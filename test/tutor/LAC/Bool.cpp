//
// Created by prophe on 2022/2/4.
//

void foo(bool some_arg);

void bar() {
    foo(true);
}

/*
TranslationUnitDecl 0x918e9b8 <<invalid sloc>> <invalid sloc>
|-FunctionDecl 0x91cfb68 <Bool.cpp:5:1, col:23> col:6 used foo 'void (bool)'
| `-ParmVarDecl 0x91cfaa0 <col:10, col:15> col:15 some_arg 'bool'
`-FunctionDecl 0x91cfca8 <line:7:1, line:9:1> line:7:6 bar 'void ()'
  `-CompoundStmt 0x91cfe28 <col:12, line:9:1>
    `-CallExpr 0x91cfe00 <line:8:5, col:13> 'void'
      |-ImplicitCastExpr 0x91cfde8 <col:5> 'void (*)(bool)' <FunctionToPointerDecay>
      | `-DeclRefExpr 0x91cfda0 <col:5> 'void (bool)' lvalue Function 0x91cfb68 'foo' 'void (bool)'
      `-CXXBoolLiteralExpr 0x91cfd90 <col:9> 'bool' true
 */