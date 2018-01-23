#include "lib.h"
#include "priv.h"
#include <stdio.h>

void uncalled_fun0() {
    printf("actually doing nothing but printing\n");
}

void uncalled_fun1() {
    printf("actually doing nothing but printing\n");
}

void f1() {
    priv_raise(1, CAP_CHOWN);
    f2();
    priv_lower(1, CAP_CHOWN);
    printf("hello from f1()\n");
    f3();
}
void f2() {
    printf("hello from f2()\n");
    f5();
    f5();
    /* f2(); */
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
    int i = 0;

    while (i < 10) {
        i++;
        /* f4(); */
    }

    priv_raise(2, CAP_CHOWN, CAP_FOWNER);
    // .....
    priv_lower(2, CAP_CHOWN, CAP_FOWNER);
}


