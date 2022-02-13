//
// Created by prophe on 2022/2/6.
//

int foo() {
    int a = 10;

    for (int Unused = 0; Unused < 20; Unused++) {
        a++;
        a--;
        a += 123;
    }

    for (int unuseD = 0, unusED = 0; unusED < 20; unusED++) {
        a++;
        a--;
        a += 123;
    }

    return a;
}