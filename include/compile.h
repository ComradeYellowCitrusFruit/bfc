#ifndef BRAINFUCK_COMPILER_COMPILE_H__
#define BRAINFUCK_COMPILER_COMPILE_H__

#include <stdio.h>
#include <stdint.h>

void compile_optimized(void *procbuf, size_t size, FILE *out);
void compile(void *procbuf, size_t size, FILE *out);

#endif
