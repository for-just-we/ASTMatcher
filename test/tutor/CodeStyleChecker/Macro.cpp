//
// Created by prophe on 2022/2/2.
//

#define clang_tutor_class_ok(class_name) class ClangTutor##class_name
#define clang_tutor_class_underscore(class_name) class Clang_TutorClass##class_name
#define clang_tutor_class_lower_case(class_name) class clangTutorClass##class_name

#define clang_tutor_func_ok(func_name) void clangTutorFunc##func_name()
#define clang_tutor_func_underscore(func_name) void clangTutor_Func##func_name()
#define clang_tutor_func_upper_case(func_name) void ClangTutorFunc##func_name()

clang_tutor_class_ok(Foo) { // equals to ClangTutorFoo{};
};
// expected-warning@+1 {{`_` in names is not allowed}}
clang_tutor_class_underscore(Bar) { // equals to Clang_TutorClassBar{};, CXXRecordDecl,16:1
};
// expected-warning@+1 {{Type and variable names should start with upper-case letter}}
clang_tutor_class_lower_case(Bar) { // equals to clangTutorClassBar{}; CXXRecordDecl,19:1
};

clang_tutor_func_ok(Foo) { // clangTutorFuncFoo(){}
};
// expected-warning@+1 {{`_` in names is not allowed}}
clang_tutor_func_underscore(Bar) { // clangTutor_FuncBar(){}  FunctionDecl,25:1
};
// expected-warning@+1 {{Function names should start with lower-case letter}}
clang_tutor_func_upper_case(Bar) { // ClangTutorFuncBar(){}  FunctionDecl,28:1
};

/*
TranslationUnitDecl 0x273ad38 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x273b630 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x273b2d0 '__int128'
|-TypedefDecl 0x273b6a0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x273b2f0 'unsigned __int128'
|-TypedefDecl 0x273ba18 <<invalid sloc>> <invalid sloc> implicit __NSConstantString '__NSConstantString_tag'
| `-RecordType 0x273b790 '__NSConstantString_tag'
|   `-CXXRecord 0x273b6f8 '__NSConstantString_tag'
|-TypedefDecl 0x273bab0 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x273ba70 'char *'
|   `-BuiltinType 0x273add0 'char'
|-TypedefDecl 0x277c1a8 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
| `-ConstantArrayType 0x277c150 '__va_list_tag [1]' 1
|   `-RecordType 0x273bba0 '__va_list_tag'
|     `-CXXRecord 0x273bb08 '__va_list_tag'
|-CXXRecordDecl 0x277c200 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:5:42, line:14:1> <scratch space>:2:1 class ClangTutorFoo definition
| |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
| | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
| | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveConstructor exists simple trivial needs_implicit
| | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveAssignment exists simple trivial needs_implicit
| | `-Destructor simple irrelevant trivial needs_implicit
| `-CXXRecordDecl 0x277c318 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:5:42, <scratch space>:2:1> col:1 implicit class ClangTutorFoo
|-CXXRecordDecl 0x277c3c0 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:6:50, line:17:1> <scratch space>:3:1 class Clang_TutorClassBar definition
| |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
| | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
| | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveConstructor exists simple trivial needs_implicit
| | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveAssignment exists simple trivial needs_implicit
| | `-Destructor simple irrelevant trivial needs_implicit
| `-CXXRecordDecl 0x277c4d8 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:6:50, <scratch space>:3:1> col:1 implicit class Clang_TutorClassBar
|-CXXRecordDecl 0x277c580 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:7:50, line:20:1> <scratch space>:4:1 class clangTutorClassBar definition
| |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
| | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
| | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveConstructor exists simple trivial needs_implicit
| | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveAssignment exists simple trivial needs_implicit
| | `-Destructor simple irrelevant trivial needs_implicit
| `-CXXRecordDecl 0x277c698 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:7:50, <scratch space>:4:1> col:1 implicit class clangTutorClassBar
|-FunctionDecl 0x277c788 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:9:40, line:23:1> <scratch space>:5:1 clangTutorFuncFoo 'void ()'
| `-CompoundStmt 0x277c870 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:22:26, line:23:1>
|-EmptyDecl 0x277c880 <col:2> col:2
|-FunctionDecl 0x277c8c8 <line:10:48, line:26:1> <scratch space>:6:1 clangTutor_FuncBar 'void ()'
| `-CompoundStmt 0x277c968 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:25:34, line:26:1>
|-EmptyDecl 0x277c978 <col:2> col:2
|-FunctionDecl 0x277c9c0 <line:11:48, line:29:1> <scratch space>:7:1 ClangTutorFuncBar 'void ()'
| `-CompoundStmt 0x277ca60 </home/prophe/projects/c/ClangCheckers/ASTMatcher/test/CodeStyleChecker/Macro.cpp:28:34, line:29:1>
`-EmptyDecl 0x277ca70 <col:2> col:2
 */