#ifndef BRAINFUCK_COMPILER_COMPILE_H__
#define BRAINFUCK_COMPILER_COMPILE_H__

#include <stdio.h>
#include <stdint.h>

extern char **symbols;
extern char *nextsym;
extern int symcount;
extern int symnum;

void compile_optimized(uint8_t *procbuf, size_t size, FILE *out);
void compile(uint8_t *procbuf, size_t size, FILE *out);

#endif
