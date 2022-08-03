#ifndef PRIM_H
#define PRIM_H

typedef struct { const char *s; double (*f)(double, double); } Prim;
extern Prim prim[];

double f_eval(double args, double e);
double f_quote(double t, double);
double f_cons(double t, double e);
double f_car(double t, double e);
double f_cdr(double t, double e);
double f_add(double t, double e);
double f_sub(double t, double e);
double f_mul(double t, double e);
double f_div(double t, double e);
double f_int(double t, double e);
double f_lt(double t, double e);
double f_eq(double t, double e);
double f_not(double t, double e);
double f_or(double t, double e);
double f_and(double t, double e);
double f_cond(double t, double e);
double f_if(double t, double e);
double f_let(double t, double e);
double f_leta(double t, double e);
double f_letreca(double t, double e);
double f_lambda(double t, double e);
double f_define(double t, double e);
double f_assoc(double t, double e);
double f_env(double, double e);
double f_heap(double, double);
double f_stack(double, double);
double f_setq(double t, double e);
double f_setcar(double t, double e);
double f_setcdr(double t, double e);
double f_macro(double t, double e);
double f_print(double t, double e);
double f_read(double t, double e);
#endif
