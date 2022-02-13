//
// Created by prophe on 2022/2/2.
//

// expected-warning@+1 {{`_` in names is not allowed}}
class ClangTutorClass_Bad;
class ClangTutorClassOK;

// expected-warning@+1 {{`_` in names is not allowed}}
struct ClangTutorStruct_Bad;
struct ClangTutorStructOK;

// expected-warning@+1 {{`_` in names is not allowed}}
union ClangTutorUnion_Bad;
union ClangTutorUnionOK;

// expected-warning@+1 {{`_` in names is not allowed}}
int ClangTutorInt_Bad;
int ClangTutorIntOK;

// expected-warning@+1 {{`_` in names is not allowed}}
void clangTutor_Bad();
void clangTutorOK();

struct SomeClass {
    // expected-warning@+1 {{`_` in names is not allowed}}
    void clangTutorMemberMethod_Bad(); //CXXMethodDecl : public FunctionDecl
    void clangTutorMemberMethodOK();

    // expected-warning@+1 {{`_` in names is not allowed}}
    int ClangTutorInt_Bad;
    int ClangTutorIntOK;
};