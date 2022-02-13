//
// Created by prophe on 2022/2/2.
//

// 1. Verify that anonymous unions are not flagged as invalid (no name ->
// nothing to check). However, the member variables _are_ verified.

static union {
// expected-warning@+1 {{Type and variable names should start with upper-case letter}}, FieldDecl
    int clangTutorBad;
    const char *ClangTutorOk;
};

// 2. Verify that anonymous function parameters are not flagged as invalid (no name
// -> nothing to check). However, the function name's _is_ verified.

void clangTutorFunc(void *); // FunctionDecl

// expected-warning@+1 {{Function names should start with lower-case letter}}
void ClangTutorFunc(void *); // FunctionDecl

// 3. Verify that anonymous bit-fields are not flagged as invalid (no name
// -> nothing to check). However, the named fields _are_ verified.
struct ClangTutorStruct { // CXXRecordDecl
    unsigned short
    // expected-warning@+1 {{Type and variable names should start with upper-case letter}}
    clangTutorBitFieldBad : 1, // FieldDecl
            ClangTutorBitFieldOK  : 1,
            : 1;
};

/*
TranslationUnitDecl 0x2d16d38 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x2d17630 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x2d172d0 '__int128'
|-TypedefDecl 0x2d176a0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x2d172f0 'unsigned __int128'
|-TypedefDecl 0x2d17a18 <<invalid sloc>> <invalid sloc> implicit __NSConstantString '__NSConstantString_tag'
| `-RecordType 0x2d17790 '__NSConstantString_tag'
|   `-CXXRecord 0x2d176f8 '__NSConstantString_tag'
|-TypedefDecl 0x2d17ab0 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x2d17a70 'char *'
|   `-BuiltinType 0x2d16dd0 'char'
|-TypedefDecl 0x2d58608 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
| `-ConstantArrayType 0x2d585b0 '__va_list_tag [1]' 1
|   `-RecordType 0x2d17ba0 '__va_list_tag'
|     `-CXXRecord 0x2d17b08 '__va_list_tag'
|-CXXRecordDecl 0x2d58660 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8, line:12:1> line:8:8 union definition
| |-DefinitionData is_anonymous pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal has_variant_members
| | |-DefaultConstructor exists trivial
| | |-CopyConstructor simple trivial has_const_param implicit_has_const_param
| | |-MoveConstructor exists simple trivial
| | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveAssignment exists simple trivial needs_implicit
| | `-Destructor simple irrelevant trivial needs_implicit
| |-FieldDecl 0x2d58790 <line:10:5, col:9> col:9 clangTutorBad 'int'
| |-FieldDecl 0x2d587f8 <line:11:5, col:17> col:17 ClangTutorOk 'const char *'
| |-CXXConstructorDecl 0x2d58938 <line:8:8> col:8 implicit used  'void () noexcept' inline default trivial
| | `-CompoundStmt 0x2d58dc8 <col:8>
| |-CXXConstructorDecl 0x2d58a68 <col:8> col:8 implicit constexpr  'void (const (anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8) &)' inline default trivial noexcept-unevaluated 0x2d58a68
| | `-ParmVarDecl 0x2d58b78 <col:8> col:8 'const (anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8) &'
| `-CXXConstructorDecl 0x2d58c18 <col:8> col:8 implicit constexpr  'void ((anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8) &&)' inline default trivial noexcept-unevaluated 0x2d58c18
|   `-ParmVarDecl 0x2d58d28 <col:8> col:8 '(anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8) &&'
|-VarDecl 0x2d588b8 <col:1, col:8> col:8 implicit '(anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8)' static callinit
| `-CXXConstructExpr 0x2d58dd8 <col:8> '(anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8)' 'void () noexcept'
|-IndirectFieldDecl 0x2d58e68 <line:10:9> col:9 implicit clangTutorBad 'int'
| |-Var 0x2d588b8 '' '(anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8)'
| `-Field 0x2d58790 'clangTutorBad' 'int'
|-IndirectFieldDecl 0x2d58ec0 <line:11:17> col:17 implicit ClangTutorOk 'const char *'
| |-Var 0x2d588b8 '' '(anonymous union at /home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Anonymous.cpp:8:8)'
| `-Field 0x2d587f8 'ClangTutorOk' 'const char *'
|-FunctionDecl 0x2d58fe8 <line:17:1, col:27> col:6 clangTutorFunc 'void (void *)'
| `-ParmVarDecl 0x2d58f20 <col:21, col:26> col:27 'void *'
|-FunctionDecl 0x2d59180 <line:20:1, col:27> col:6 ClangTutorFunc 'void (void *)'
| `-ParmVarDecl 0x2d590f0 <col:21, col:26> col:27 'void *'
`-CXXRecordDecl 0x2d59228 <line:24:1, line:30:1> line:24:8 struct ClangTutorStruct definition
  |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal
  | |-DefaultConstructor exists trivial needs_implicit
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl 0x2d59348 <col:1, col:8> col:8 implicit struct ClangTutorStruct
  |-FieldDecl 0x2d59430 <line:25:5, line:27:29> col:5 clangTutorBitFieldBad 'unsigned short'
  | `-ConstantExpr 0x2d59410 <col:29> 'int'
  |   |-value: Int 1
  |   `-IntegerLiteral 0x2d593d8 <col:29> 'int' 1
  |-FieldDecl 0x2d86750 <line:25:5, line:28:37> col:13 ClangTutorBitFieldOK 'unsigned short'
  | `-ConstantExpr 0x2d594b8 <col:37> 'int'
  |   |-value: Int 1
  |   `-IntegerLiteral 0x2d59480 <col:37> 'int' 1
  `-FieldDecl 0x2d867f8 <line:25:5, line:29:15> col:13 'unsigned short'
    `-ConstantExpr 0x2d867d8 <col:15> 'int'
      |-value: Int 1
      `-IntegerLiteral 0x2d867a0 <col:15> 'int' 1
 */