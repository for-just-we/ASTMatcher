//
// Created by prophe on 2022/2/2.
//

// expected-warning@+1 {{Function names should start with lower-case letter}}
void ClangTutorFuncBad(); //FunctionDecl

void clangTutorFuncOK();

struct ClangTutorStruct { //CXXRecordDecl
    // expected-warning@+1 {{Function names should start with lower-case letter}}
    void ClangTutorMemberMethodBad(); // CXXMethodDecl
    void clangTutorMemberMethodOK(); // CXXMethodDecl
};

/*
TranslationUnitDecl 0x3ddad38 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x3ddb630 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x3ddb2d0 '__int128'
|-TypedefDecl 0x3ddb6a0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x3ddb2f0 'unsigned __int128'
|-TypedefDecl 0x3ddba18 <<invalid sloc>> <invalid sloc> implicit __NSConstantString '__NSConstantString_tag'
| `-RecordType 0x3ddb790 '__NSConstantString_tag'
|   `-CXXRecord 0x3ddb6f8 '__NSConstantString_tag'
|-TypedefDecl 0x3ddbab0 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x3ddba70 'char *'
|   `-BuiltinType 0x3ddadd0 'char'
|-TypedefDecl 0x3e1bc88 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
| `-ConstantArrayType 0x3e1bc30 '__va_list_tag [1]' 1
|   `-RecordType 0x3ddbba0 '__va_list_tag'
|     `-CXXRecord 0x3ddbb08 '__va_list_tag'
|-FunctionDecl 0x3e1bd28 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Function.cpp:6:1, col:24> col:6 ClangTutorFuncBad 'void ()'
|-FunctionDecl 0x3e1be30 <line:8:1, col:23> col:6 clangTutorFuncOK 'void ()'
`-CXXRecordDecl 0x3e1bed0 <line:10:1, line:14:1> line:10:8 struct ClangTutorStruct definition
  |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
  | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
  | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveConstructor exists simple trivial needs_implicit
  | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
  | |-MoveAssignment exists simple trivial needs_implicit
  | `-Destructor simple irrelevant trivial needs_implicit
  |-CXXRecordDecl 0x3e1bfe8 <col:1, col:8> col:8 implicit struct ClangTutorStruct
  |-CXXMethodDecl 0x3e1c0c8 <line:12:5, col:36> col:10 ClangTutorMemberMethodBad 'void ()'
  `-CXXMethodDecl 0x3e1c188 <line:13:5, col:35> col:10 clangTutorMemberMethodOK 'void ()'
 */