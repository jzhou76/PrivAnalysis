#include <stdio.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include "lib.h"
#include "priv.h"

void foo(int a);

void indirect_foo(int a, char (*ifunc0)(float, long), int (*ifunc1)(int)) {
    ifunc1(10);
    int i = 0;
    printf("i = %d\n", i);
    f5();
    ifunc0(20, 30);

    priv_raise(1, CAP_CHOWN);
    printf("~~~~\n");
    priv_lower(1, CAP_CHOWN);

    f5();

    printf("a = %d\n", a);
    print_cap();

    int j = a;
    while (j-- > 1) {
        printf("this while will be deleted from CFG\n");
        f3();
        
        /* break; */
    }
}

void foo(int a) {
    priv_raise(1, CAP_CHOWN);
    printf("~~~~\n");
    priv_lower(1, CAP_CHOWN);

    f5();

    printf("a = %d\n", a);
    print_cap();

    int i = a;
    while (i-- > 1) {
        printf("this while will be deleted from CFG\n");
        f3();
        
        /* break; */
    }

    /* while (a > 1) { */
    /*     priv_raise(1, CAP_KILL); */
    /*     priv_lower(1, CAP_KILL); */

    /*     f5(); */

    /*     if (a > 1) middle_priv_func(); */
    /*     else break; */
    /* } */

    /* return; */
}

// an "external" function
char baz(float a, long b) {
    int c = (int)a + (int)b;
    foo(c);
    return 'c';
}

int bar(int b) {
    priv_raise(1, CAP_CHOWN);
    foo(b);
    priv_lower(1, CAP_CHOWN);
    /* some_fun0(); */
    return 0;
}

int main(int argc, char *argv[]) {
    printf("hello from test\n");
    foo(argc);
    
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

    // call a function with function pointers
    indirect_foo(argc, &baz, &bar);

    return 0;
}


