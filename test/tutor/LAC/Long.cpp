//
// Created by prophe on 2022/2/4.
//


void foo(int a, float b, char c, const char* d, bool e);
void foo1(int a, int b);
void foo2(int a);

void bar() {
    foo(1, 1.0, '1', "one", true);
    foo1(2, 3);
}

/*
TranslationUnitDecl 0x8b489b8 <<invalid sloc>> <invalid sloc>
|-FunctionDecl 0x8b89d98 <Long.cpp:5:1, col:55> col:6 used foo 'void (int, float, char, const char *, bool)'
| |-ParmVarDecl 0x8b89aa8 <col:10, col:14> col:14 a 'int'
| |-ParmVarDecl 0x8b89b28 <col:17, col:23> col:23 b 'float'
| |-ParmVarDecl 0x8b89ba0 <col:26, col:31> col:31 c 'char'
| |-ParmVarDecl 0x8b89c20 <col:34, col:46> col:46 d 'const char *'
| `-ParmVarDecl 0x8b89c98 <col:49, col:54> col:54 e 'bool'
`-FunctionDecl 0x8b89ef8 <line:7:1, line:9:1> line:7:6 bar 'void ()'
  `-CompoundStmt 0x8b8a1c8 <col:12, line:9:1>
    `-CallExpr 0x8b8a150 <line:8:5, col:33> 'void'
      |-ImplicitCastExpr 0x8b8a138 <col:5> 'void (*)(int, float, char, const char *, bool)' <FunctionToPointerDecay>
      | `-DeclRefExpr 0x8b8a0e8 <col:5> 'void (int, float, char, const char *, bool)' lvalue Function 0x8b89d98 'foo' 'void (int, float, char, const char *, bool)'
      |-IntegerLiteral 0x8b89fe0 <col:9> 'int' 1
      |-ImplicitCastExpr 0x8b8a198 <col:12> 'float' <FloatingCast>
      | `-FloatingLiteral 0x8b8a000 <col:12> 'double' 1.000000e+00
      |-CharacterLiteral 0x8b8a020 <col:17> 'char' 49
      |-ImplicitCastExpr 0x8b8a1b0 <col:22> 'const char *' <ArrayToPointerDecay>
      | `-StringLiteral 0x8b8a0b8 <col:22> 'const char [4]' lvalue "one"
      `-CXXBoolLiteralExpr 0x8b8a0d8 <col:29> 'bool' true
 */