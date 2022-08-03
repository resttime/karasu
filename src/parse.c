#include <stdio.h>
#include <string.h>

#include "builtin.h"
#include "parse.h"

char buf[40],see = ' ';
void look() {
    int c = getchar();
    if (c == EOF) {
        freopen("/dev/tty", "t", stdin);
        c = ' ';
    }
    see = c;
}
uint32_t seeing(char c) { return c == ' ' ? see > 0 && see <= c : see == c; }
char get() { char c = see; look(); return c; }
char scan() {
    uint32_t i = 0;
s:  while(seeing(' ')) look();
    if (seeing(';')) {
        do {
            look();
        } while (!seeing('\n'));
        goto s;
    }
    if (seeing('(') || seeing(')') || seeing('\'')) {
        buf[i++] = get();
    } else {
        do {
            buf[i++] = get();
        } while (i < 39 && !seeing('(') && !seeing(')') && !seeing(' '));
    }
    buf[i] = 0;
    return *buf;
}
double read() {
    scan();
    return parse();
}
double parse() {
    return *buf == '(' ? list() : *buf== '\'' ? quote() : atomic();
}

double list() {
    double x;
    return scan() == ')' ? nil :
        !strcmp(buf, ".") ? (x = read(), scan(), x) :
        (x = parse(), cons(x,list()));
}

double quote() { return cons(atom("quote"), cons(read(), nil)); }
double atomic() {
    double n;
    uint32_t i;
    return sscanf(buf, "%lg%n", &n, &i) > 0 && !buf[i] ? n : atom(buf);
}
