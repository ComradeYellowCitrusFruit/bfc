/*
*       This file is part of bfc.
*
*       bfv is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*       bfc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along with bfc. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef BRAINFUCK_COMPILER_ARGS_H__
#define BRAINFUCK_COMPILER_ARGS_H__

#include <stdbool.h>

#define OS_LINUX 0
#define OS_OPENBSD 1
#define OS_FREEBSD 2
#define OS_WIN32 -1

#ifdef __linux__
#define HOST_OS OS_LINUX
#elif defined(__OpenBSD__)
#define HOST_OS OS_OPENBSD
#elif defined(__FreeBSD__)
#define HOST_OS OS_FREEBSD
#elif defined(_WIN32)
#define HOST_OS OS_WIN32
#endif

#define OS_HOST HOST_OS

typedef struct
{
	bool optimize;
	bool procedures;
	int targ;
	int cells;
	char *outfile;
} Args_state_t;

extern Args_state_t args;

#endif
