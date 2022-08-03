#include <stdio.h>

#include "builtin.h"
#include "prim.h"
#include "print.h"

void print(double x) {
    if (T(x) == NIL) printf("()");
    else if (T(x) == ATOM) printf("%s", A+ord(x));
    else if (T(x) == PRIM) printf("<%s>", prim[ord(x)].s);
    else if (T(x) == CONS) printlist(x);
    else if (T(x) == CLOS) printf("{%u}", ord(x));
    else printf("%.10lg", x);
}

void printtag(double x) {
    if (T(x) == NIL) printf("NIL ");
    else if (T(x) == ATOM) printf("ATOM");
    else if (T(x) == PRIM) printf("PRIM");
    else if (T(x) == CONS) printf("CONS");
    else if (T(x) == CLOS) printf("CLOS");
    else printf("    ");
}

void printlist(double t) {
    putchar('(');
    while (1) {
        print(car(t));
        if (not(t=cdr(t))) break;
        if (T(t) != CONS) {
            printf(" . ");
            print(t);
            break;
        }
        putchar(' ');
    }
    putchar(')');
}
