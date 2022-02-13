//
// Created by prophe on 2022/2/2.
//

// expected-warning@+1 {{Type and variable names should start with upper-case letter}}
class clangTutorClassBad; //CXXRecordDecl
class ClangTutorClassOK;

// expected-warning@+1 {{Type and variable names should start with upper-case letter}}
struct clangTutorStructBad; //CXXRecordDecl
struct ClangTutorStructOK;

// expected-warning@+1 {{Type and variable names should start with upper-case letter}}
union clangTutorUnionBad; //CXXRecordRecl
union ClangTutorUnionOK;

// expected-warning@+1 {{Type and variable names should start with upper-case letter}}
int clangTutorIntBad; //VarDecl
int ClangTutorIntOK;

struct SomeClass {
    // expected-warning@+1 {{Type and variable names should start with upper-case letter}}
    int clangTutorMemberVarBad; //FieldDecl
    int ClangTutorMemberVarOK;
};

/*
TranslationUnitDecl 0x2aeed78 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x2aef670 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x2aef310 '__int128'
|-TypedefDecl 0x2aef6e0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x2aef330 'unsigned __int128'
|-TypedefDecl 0x2aefa58 <<invalid sloc>> <invalid sloc> implicit __NSConstantString '__NSConstantString_tag'
| `-RecordType 0x2aef7d0 '__NSConstantString_tag'
|   `-CXXRecord 0x2aef738 '__NSConstantString_tag'
|-TypedefDecl 0x2aefaf0 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x2aefab0 'char *'
|   `-BuiltinType 0x2aeee10 'char'
|-TypedefDecl 0x2b2fcc8 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
| `-ConstantArrayType 0x2b2fc70 '__va_list_tag [1]' 1
|   `-RecordType 0x2aefbe0 '__va_list_tag'
|     `-CXXRecord 0x2aefb48 '__va_list_tag'
|-CXXRecordDecl 0x2b2fd20 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/TypesAndVars.cpp:6:1, col:7> col:7 class clangTutorClassBad
|-CXXRecordDecl 0x2b2fdd0 <line:7:1, col:7> col:7 class ClangTutorClassOK
|-CXXRecordDecl 0x2b2fe80 <line:10:1, col:8> col:8 struct clangTutorStructBad
|-CXXRecordDecl 0x2b2ff30 <line:11:1, col:8> col:8 struct ClangTutorStructOK
|-CXXRecordDecl 0x2b2ffe0 <line:14:1, col:7> col:7 union clangTutorUnionBad
|-CXXRecordDecl 0x2b30090 <line:15:1, col:7> col:7 union ClangTutorUnionOK
|-VarDecl 0x2b30158 <line:18:1, col:5> col:5 clangTutorIntBad 'int'
|-VarDecl 0x2b30220 <line:19:1, col:5> col:5 ClangTutorIntOK 'int'
`-CXXRecordDecl 0x2b30288 <line:21:1, line:25:1> line:21:8 struct SomeClass definition
  |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal
  | |-DefaultConstructor exists trivial needs_implicit
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl 0x2b303a8 <col:1, col:8> col:8 implicit struct SomeClass
  |-FieldDecl 0x2b30450 <line:23:5, col:9> col:9 clangTutorMemberVarBad 'int'
  `-FieldDecl 0x2b304b8 <line:24:5, col:9> col:9 ClangTutorMemberVarOK 'int'
 */