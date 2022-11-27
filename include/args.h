#ifndef BRAINFUCK_COMPILER_ARGS_H__
#define BRAINFUCK_COMPILER_ARGS_H__

#include <stdbool.h>

typedef struct
{
	bool optimize;
	char *outfile;
} Args_state_t;

extern Args_state_t args;

#endif
