#ifndef BRAINFUCK_COMPILER_ARGS_H__
#define BRAINFUCK_COMPILER_ARGS_H__

#include <stdbool.h>

#define BITS8 8
#define BITS16 16
#define BITS32 32

typedef struct
{
	bool optimize;
	int bits;
	bool gotos;
	char *outfile;
} Args_state_t;

extern Args_state_t args;

#endif
