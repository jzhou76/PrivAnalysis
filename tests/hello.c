#include <stdio.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include "lib.h"
#include "priv.h"

void foo(int a);

void indirect_foo(int a, char (*fn)(int), char (*f2)(int)) {
    fn(10);
    f2(20);
}

void foo(int a) {
    priv_raise(1, CAP_CHOWN);
    f5();
    priv_lower(1, CAP_CHOWN);

    printf("a = %d\n", a);
    print_cap();
    return;
}

// an "external" function
char baz(int a) {
    int b = a + 1;
    foo(b);
    return 'c';
}

void bar(int b) {
    priv_raise(1, CAP_CHOWN);
    foo(b);
    priv_lower(1, CAP_CHOWN);
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

    // call a function with function pointers
    indirect_foo(10, &baz, &baz);

    return 0;
}


