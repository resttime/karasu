#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "builtin.h"
#include "parse.h"
#include "prim.h"
#include "print.h"

Prim prim[] = {
    {"eval", f_eval},
    {"quote", f_quote},
    {"cons", f_cons},
    {"car", f_car},
    {"cdr", f_cdr},
    {"+", f_add},
    {"-", f_sub},
    {"*", f_mul},
    {"/", f_div},
    {"int", f_int},
    {"<", f_lt},
    {"eq?", f_eq},
    {"or", f_or},
    {"and", f_and},
    {"not", f_not},
    {"cond", f_cond},
    {"if", f_if},
    {"let", f_let},
    {"let*", f_leta},
    {"letrec*", f_letreca},
    {"lambda", f_lambda},
    {"define", f_define},
    {"assoc", f_assoc},
    {"env", f_env},
    {"heap", f_heap},
    {"stack", f_stack},
    {"setq", f_setq},
    {"set-car!", f_setcar},
    {"set-cdr!", f_setcdr},
    {"macro", f_macro},
    {"print", f_print},
    {"read", f_read},
    {0}
};

double f_eval(double t, double e) { return eval(car(evlis(t,e)),e); }
double f_quote(double t, double _) { (void)_; return car(t); }
double f_cons(double t, double e) { return evlis(t,e), cons(car(t), car(cdr(t))); }
double f_car(double t, double e) { return car(car(evlis(t,e))); }
double f_cdr(double t, double e) { return cdr(car(evlis(t,e))); }

double f_add(double t, double e) {
    double n = car(t = evlis(t,e));
    while (!not(t = cdr(t))) n += car(t);
    return num(n);
}
double f_sub(double t, double e) {
    double n = car(t = evlis(t,e));
    while (!not(t = cdr(t))) n -= car(t);
    return num(n);
}
double f_mul(double t, double e) {
    double n = car(t = evlis(t,e));
    while (!not(t = cdr(t))) n *= car(t);
    return num(n);
}
double f_div(double t, double e) {
    double n = car(t = evlis(t,e));
    while (!not(t = cdr(t))) n /= car(t);
    return num(n);
}
double f_int(double t, double e) {
    double n = car(evlis(t,e));
    return n-1e9 < 0 && n+1e9 > 0 ? (long)n : n;
}
double f_lt(double t, double e) {
    return t = evlis(t,e),car(t) - car(cdr(t)) < 0 ? tru : nil;
}
double f_eq(double t, double e) {
    return t = evlis(t,e),equ(car(t), car(cdr(t))) < 0 ? tru : nil;
}
double f_not(double t, double e) { return not(car(evlis(t,e))) ? tru : nil; }
double f_or(double t, double e) {
    for (; T(t) != NIL; t = cdr(t)) if (!not(eval(car(t),e))) return tru;
    return nil;
}
double f_and(double t, double e) {
    for (; T(t) != NIL; t = cdr(t)) if (not(eval(car(t),e))) return nil;
    return tru;
}
double f_cond(double t, double e) {
    while (T(t) != NIL && not(eval(car(car(t)),e))) t = cdr(t);
    return eval(car(cdr(car(t))),e);
}
double f_if(double t, double e) {
    return eval(car(cdr(not(eval(car(t),e)) ? cdr(t) : t)),e);
}
double f_leta(double t, double e) {
    while (T(t) != NIL && !not(cdr(t))) {
        e = pair(car(car(t)), eval(car(cdr(car(t))),e),e);
        t = cdr(t);
    }
    return eval(car(t), e);
}
double f_letreca(double t, double e) {
    while (T(t) != NIL && !not(cdr(t))) {
        e = pair(car(car(t)), err, e);
        cell[sp+2] = eval(car(cdr(car(t))),e);
        t = cdr(t);
    }
    return eval(car(t), e);
}
double f_let(double t, double e) {
    double d = e;
    while (T(t) != NIL && !not(cdr(t))) {
        d = pair(car(car(t)), eval(car(cdr(car(t))),e),d);
        t = cdr(t);
    }
    return eval(car(t), d);
}
double f_lambda(double t, double e) { return closure(car(t),car(cdr(t)),e); }

double f_define(double t, double e) {
    env = pair(car(t),eval(car(cdr(t)),e),env);
    return car(t);
}

double f_assoc(double t, double e) {
    t = evlis(t, e);
    return assoc(car(t), car(cdr(t)));
}
double f_env(double _t, double e) { (void)_t; return e; }

double f_heap(double _t, double _e) {
    (void)_t; (void)_e;
    uint32_t i = 0;
    while (i < hp) {
        printf("BYTE %04u: %s\n", i, A+i);
        i += strlen(A+i)+1;
    }
    return i;
}

double f_stack(double _t, double _e) {
    (void)_t; (void)_e;
    uint32_t i = N;
    while (--i > sp) {
        printf("CELL %04u: ", i);
        printtag(cell[i]);
        printf(" : 0x%" PRIx64 " : ", *(uint64_t*)&cell[i]);
        print(cell[i]);
        printf("\n");
    }
    return num(i);
}

double f_setq(double t, double e) {
    double a = car(t);
    double x = eval(car(cdr(t)),e);
    while (T(e) == CONS && !equ(a, car(car(e)))) e = cdr(e);
    return T(e) == CONS ? cell[ord(car(e))] = x : err;
}

double f_setcar(double t, double e) {
    double p = car(t = evlis(t,e));
    return (T(p) == CONS) ? cell[ord(p)+1] = car(cdr(t)) : err;
}

double f_setcdr(double t, double e) {
    double p = car(t = evlis(t,e));
    return (T(p) == CONS) ? cell[ord(p)] = car(cdr(t)) : err;
}

double f_macro(double t, double _e) { (void)_e; return macro(car(t), car(cdr(t))); }

double f_read(double _t, double _e) {
    (void)_t; (void)_e;
    double x;
    char c = see;
    see = ' ';
    x = read();
    see = c;
    return x;
}

double f_print(double t, double e) {
    for (t = evlis(t,e); T(t) != NIL; t = cdr(t)) print(car(t));
    return nil;
}
