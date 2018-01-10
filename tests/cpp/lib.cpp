#include "lib.h"
#include <stdio.h>

namespace lib {

void uncalled_fun0() {
    printf("actually doing nothing but printing\n");
}

void uncalled_fun1() {
    printf("actually doing nothing but printing\n");
}

void f1() {
    f2();
    printf("hello from f1()\n");
    f3();
}
void f2() {
    printf("hello from f2()\n");
    f5();
    f5();
}

void f3() {
    printf("hello from f3()\n");
    f4();
}

void f4() {
    printf("hello from f4()\n");
    f1();
}

void f5() {
    /* printf("hello from f5()\n"); */
}

}
