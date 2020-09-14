#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern int getword(char* s, int limit, FILE *stream);
extern void* emalloc(size_t n);
extern void* erealloc(char* a, size_t n);

#endif
