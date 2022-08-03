#include <stdio.h>

#include "builtin.h"
#include "print.h"
#include "parse.h"
#include "prim.h"

int main() {
    nil = box(NIL,0);
    tru = atom("#t");
    err = atom("ERR");
    env = pair(tru, tru, nil);
    for (int i=0; prim[i].s; ++i) {
        env = pair(atom(prim[i].s),box(PRIM, i), env);
    }
    while (1) {
        printf("\n%u>", sp-hp/8);
        print(eval(read(), env));
        gc();
    }
    return 0;
}
