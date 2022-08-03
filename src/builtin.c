#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "prim.h"

double cell[N];
uint32_t hp = 0, sp = N;

uint32_t ATOM=0x7ff8,PRIM=0x7ff9,CONS=0x7ffa,CLOS=0x7ffb,MACR=0x7ffc,NIL=0x7ffd;

double nil,tru,err;

double env;

double box(uint32_t tag, uint32_t i) {
    double x;
    *(uint64_t*)&x = (uint64_t)tag<<48|i;
    return x;
}
uint32_t ord(double x) { return *(uint64_t*)&x; }
double num(double n) { return n; }

uint32_t equ(double x, double y) { return *(uint64_t*)&x == *(uint64_t*)&y; }
uint32_t not(double x) { return T(x) == NIL; }
double atom(const char *s) {
    uint32_t i = 0;
    while (i < hp && strcmp(A+i, s)) i += strlen(A+i)+1;
    if (i == hp && (hp += strlen(strcpy(A+i, s))+1) > sp<<3) abort();
    return box(ATOM, i);
}
double cons(double x, double y) {
    cell[--sp] = x;
    cell[--sp] = y;
    if (hp > sp<<3) abort();
    return box(CONS,sp);
}
double car(double p) { return T(p) == CONS || T(p) == CLOS || T(p) == MACR ? cell[ord(p)+1] : err; }
double cdr(double p) { return T(p) == CONS || T(p) == CLOS || T(p) == MACR ? cell[ord(p)] : err; }
double pair(double v, double x, double e) { return cons(cons(v,x),e); }
double closure(double v, double x, double e) {
    return box(CLOS, ord(pair(v,x,equ(e,env) ? nil : e)));
}
double assoc(double v, double e) {
    while (T(e) == CONS && !equ(v, car(car(e)))) e = cdr(e);
    return T(e) == CONS ? cdr(car(e)) : err;
}
double reduce(double closure, double args, double e) {
    return eval(cdr(car(closure)),
                bind(car(car(closure)),
                     evlis(args, e),
                     not(cdr(closure)) ? env : cdr(closure)));
}
double apply(double f, double t, double e) {
    return
        T(f) == PRIM ? prim[ord(f)].f(t,e) :
        T(f) == CLOS ? reduce(f,t,e) :
        T(f) == MACR ? expand(f,t,e) :
        err;
}
double eval(double x, double e) {
    return
        T(x) == ATOM ? assoc(x,e) :
        T(x) == CONS ? apply(eval(car(x),e),cdr(x),e):
        x;
}
double bind(double v, double args, double e) {
    return
        T(v) == NIL ? e :
        T(v) == CONS ? bind(cdr(v), cdr(args),pair(car(v),car(args),e)) :
        pair(v,args,e);
}
double evlis(double t, double e) {
    return T(t) == CONS ? cons(eval(car(t),e),evlis(cdr(t),e)) :
        eval(t,e);
}
double macro(double v, double x) { return box(MACR, ord(cons(v,x))); }
double expand(double f, double t, double e) {
    return eval(eval(cdr(f), bind(car(f), t, env)), e);
}
void gc() { sp = ord(env); }
