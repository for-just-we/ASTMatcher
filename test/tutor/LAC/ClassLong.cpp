//
// Created by prophe on 2022/2/5.
//


class Box{
public:
    float length;   // 盒子的长度
    float breadth;  // 盒子的宽度
    float height;   // 盒子的高度

    void set(float len, float bre, float hei);
};

void bar(){
    Box* b = new Box();
    b->set(1, 2, 3);
}

/*
TranslationUnitDecl 0x9318858 <<invalid sloc>> <invalid sloc>
|-CXXRecordDecl 0x9359a90 <ClassLong.cpp:6:1, line:13:1> line:6:7 referenced class Box definition
| |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal
| | |-DefaultConstructor exists trivial
| | |-CopyConstructor simple trivial has_const_param implicit_has_const_param
| | |-MoveConstructor exists simple trivial
| | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| | |-MoveAssignment exists simple trivial needs_implicit
| | `-Destructor simple irrelevant trivial needs_implicit
| |-CXXRecordDecl 0x9359ba8 <col:1, col:7> col:7 implicit class Box
| |-AccessSpecDecl 0x9359c38 <line:7:1, col:7> col:1 public
| |-FieldDecl 0x9359c78 <line:8:5, col:11> col:11 length 'float'
| |-FieldDecl 0x9359ce0 <line:9:5, col:11> col:11 breadth 'float'
| |-FieldDecl 0x9359d48 <line:10:5, col:11> col:11 height 'float'
| |-CXXMethodDecl 0x9359fc8 <line:12:5, col:46> col:10 used set 'void (float, float, float)'
| | |-ParmVarDecl 0x9359db0 <col:14, col:20> col:20 len 'float'
| | |-ParmVarDecl 0x9359e30 <col:25, col:31> col:31 bre 'float'
| | `-ParmVarDecl 0x9359eb0 <col:36, col:42> col:42 hei 'float'
| |-CXXConstructorDecl 0x9388180 <line:6:7> col:7 implicit referenced Box 'void () noexcept' inline default trivial
| |-CXXConstructorDecl 0x9388288 <col:7> col:7 implicit constexpr Box 'void (const Box &)' inline default trivial noexcept-unevaluated 0x9388288
| | `-ParmVarDecl 0x9388398 <col:7> col:7 'const Box &'
| `-CXXConstructorDecl 0x9388438 <col:7> col:7 implicit constexpr Box 'void (Box &&)' inline default trivial noexcept-unevaluated 0x9388438
|   `-ParmVarDecl 0x9388548 <col:7> col:7 'Box &&'
|-FunctionDecl 0x935a0e8 <line:15:1, line:18:1> line:15:6 bar 'void ()'
| `-CompoundStmt 0x93887b0 <col:11, line:18:1>
|   |-DeclStmt 0x9388650 <line:16:5, col:23>
|   | `-VarDecl 0x935a1e0 <col:5, col:22> col:10 used b 'Box *' cinit
|   |   `-CXXNewExpr 0x9388610 <col:14, col:22> 'Box *' Function 0x935a380 'operator new' 'void *(unsigned long)'
|   |     `-CXXConstructExpr 0x93885e8 <col:18, col:22> 'Box' 'void () noexcept' zeroing
|   `-CXXMemberCallExpr 0x9388730 <line:17:5, col:19> 'void'
|     |-MemberExpr 0x93886a0 <col:5, col:8> '<bound member function type>' ->set 0x9359fc8
|     | `-ImplicitCastExpr 0x9388688 <col:5> 'Box *' <LValueToRValue>
|     |   `-DeclRefExpr 0x9388668 <col:5> 'Box *' lvalue Var 0x935a1e0 'b' 'Box *'
|     |-ImplicitCastExpr 0x9388768 <col:12> 'float' <IntegralToFloating>
|     | `-IntegerLiteral 0x93886d0 <col:12> 'int' 1
|     |-ImplicitCastExpr 0x9388780 <col:15> 'float' <IntegralToFloating>
|     | `-IntegerLiteral 0x93886f0 <col:15> 'int' 2
|     `-ImplicitCastExpr 0x9388798 <col:18> 'float' <IntegralToFloating>
|       `-IntegerLiteral 0x9388710 <col:18> 'int' 3

 */