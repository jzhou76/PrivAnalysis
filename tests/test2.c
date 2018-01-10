#include <stdio.h>
#include "lib.h"

int same_source_func(int a, int b) {
    return a * b + a;
}

int bar(int a) {
    return a;
}

void foo(int (*f)(int)) {
    f(1);
}

int main(int argc, char *argv[]) {
    /* printf("hello\n"); */
    f5();
    same_source_func(1, 2);

    foo(&bar);
    return 0;
}
