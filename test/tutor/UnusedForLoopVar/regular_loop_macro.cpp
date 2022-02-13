//
// Created by prophe on 2022/2/6.
// visitor

#define GENERATE_UNUSED_REGULAR_FOR_LOOP_VAR(size, inc) \
  do {\
    for (int x = 0; x < size; ++x)\
      ++inc;\
  }\
  while (0)

int foo(int arg) {
    // expected-warning@+1 {{(Recursive AST Visitor) regular for-loop variable not used}}
    GENERATE_UNUSED_REGULAR_FOR_LOOP_VAR(3, arg);
    return arg;
}
