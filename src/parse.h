#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>

extern char buf[], see;
void look();
uint32_t seeing(char c);
char get();
char scan();
double read();
double parse();
double list();
double quote();
double atomic();

#endif
