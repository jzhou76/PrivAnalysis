#include <stdio.h>
#include "lib.h"

int g = 10;

int same_source_func(int a, int b) {
    return a * b + a;
}

int bar(int a) {
    return a;
}

void foo(int (*f)(int)) {
    if (g > 0) printf("g > 0");
    else printf("g <= 0");
    /* f(1); */
}

int main(int argc, char *argv[]) {
    /* printf("hello\n"); */
    f5();
    same_source_func(1, 2);

    foo(&bar);
    return 0;
}
