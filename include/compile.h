/*
*       This file is part of bfc.
*
*       bfv is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*       bfc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along with bfc. If not, see <https://www.gnu.org/licenses/>.
*/
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
