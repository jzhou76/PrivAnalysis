#include <stdio.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include "lib.h"
/* #include <string.h> */

void foo(int a) {
    f5();
    printf("a = %d\n", a);
    return;
}

void bar(int b) {
    foo(b);
    /* some_fun0(); */
    return;
}

int main(int argc, char *argv[]) {
    printf("hello from test\n");
    foo(10);
    
    f1();
    /* cap_t cap; */
    /* size_t t; */

    if (argc > 2) {
        printf("there are at least 3 arguments\n");
    } else {
        printf("there are no more than 2 arguments\n");
    }
    
    f2();
    // call function declared in lib.h
    /* some_fun0(); */
    return 0;
}


