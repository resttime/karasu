#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdint.h>

#define T(x)*(uint64_t*)&x>>48
#define A (char*)cell
#define N 1024

extern double cell[N];
extern uint32_t hp, sp;

extern uint32_t ATOM,PRIM,CONS,CLOS,MACR,NIL;

extern double nil,tru,err;

extern double env;

double box(uint32_t tag, uint32_t i);
uint32_t ord(double x);
double num(double n);

uint32_t equ(double x, double y);
uint32_t not(double x);
double atom(const char *s);
double cons(double x, double y);
double car(double p);
double cdr(double p);
double pair(double v, double x, double e);
double closure(double v, double x, double e);
double assoc(double v, double e);
double reduce(double closure, double args, double e);
double apply(double f, double t, double e);
double eval(double x, double e);
double bind(double v, double args, double e);
double evlis(double t, double e);
double macro(double v, double x);
double expand(double f, double t, double e);
void gc();
#endif
