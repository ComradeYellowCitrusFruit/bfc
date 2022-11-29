#ifndef BRAINFUCK_COMPILER_ARGS_H__
#define BRAINFUCK_COMPILER_ARGS_H__

#include <stdbool.h>

#define LINUX 0
#define OPENBSD 1
#define FREEBSD 2
#define WIN32 -1

#ifdef __linux__
#define HOST LINUX
#elif defined(__OpenBSD__)
#define HOST OPENBSD
#elif defined(__FreeBSD__)
#define HOST FREEBSD
#elif defined(_WIN32)
#define HOST WIN32
#endif

typedef struct
{
	bool optimize;
	int targ;
	char *outfile;
} Args_state_t;

extern Args_state_t args;

#endif
