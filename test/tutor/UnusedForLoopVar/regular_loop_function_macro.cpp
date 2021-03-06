//
// Created by prophe on 2022/2/6.
//
// visitor
#define GENERATE_FUNCTION_WITH_UNUSED_REGULAR_FOR_LOOP_VAR(prefix) \
  int func_##prefix(int arg) {\
    for (int x = 0; x < 5; ++x)\
      ++arg;\
    return arg;\
  }

// expected-warning@+1 {{(Recursive AST Visitor) regular for-loop variable not used}}
GENERATE_FUNCTION_WITH_UNUSED_REGULAR_FOR_LOOP_VAR(foo)