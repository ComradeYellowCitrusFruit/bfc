/*
*       This file is part of bfc.
*
*       bfc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*       bfc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along with bfc. If not, see <https://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _WIN32

#include <unistd.h>
#include <sys/syscall.h>

#else

#define SYS_read 0
#define SYS_write 1
#define SYS_exit 60

#endif

#include "include/args.h"
#include "include/compile.h"
#include "include/syscalls.h"

char **symbols;
char *nextsym;
int symcount = 0;
int symnum = 0;

void compile_optimized(uint8_t *procbuf, size_t size, FILE *out)
{
	if(args.targ != -1)
	{
		/* Initialize this shit */
		fprintf(out, \
			".text\n" \
			".globl _start\n" \
			".lcomm arr, 3000, 2\n"
			"_start:\n" \
			"\txor %%ecx, %%ecx\n" \
		, args.cells);
	}
	else
	{
		/* Initialize this shit */
		fprintf(out, \
			".text\n" \
			".globl main\n" \
			".lcomm arr, %i, 2\n"
			"main:\n" \
			"\txor %%ecx, %%ecx\n" \
		, args.cells);
	}

	symbols = malloc(1);
	nextsym = malloc(strlen(".L0") + 1);
	strcpy(nextsym, ".L0");

	int i = 0;
	while(i < size)
	{
		switch(procbuf[i])
		{
			/* TODO: Add in algorithm optimizations (ie. div-mods, multiplies, divides, mods, variable cell movements, etc.) */

			case '+':
			/* Attempt to optimize repeated increments */
			if(procbuf[i+1] == '+')
			{
				int c = 0;
				while(procbuf[i] == '+' && i < size)
				{
					c++; i++;
				}
				i--;
				fprintf(out, "\taddb $%i, arr(, %%ecx)\n", c);
			}
			/* Attempt to optimize useless adds and subtracts */
			else if(procbuf[i+1] == '-')
				break;
			else
				fprintf(out, "\tincb arr(, %%ecx)\n");
			break;

			case '-':
			/* Attempt to optimize repeated decrements */
			if(procbuf[i+1] == '-')
			{
				int c = 0;
				while(procbuf[i] == '-' && i < size)
				{
					c++; i++;
				}
				fprintf(out, "\tsubb $%i, arr(, %%ecx)\n", c);
			}
			/* Attempt to optimize useless adds and subtracts */
			else if(procbuf[i+1] == '+')
				break;
			else
				fprintf(out, "\tdecb arr(, %%ecx)\n");
			break;

			case '>':
            /* Attempt to optimize repeated increments */
            if(procbuf[i+1] == '>')
            {
                int c = 0;
                while(procbuf[i] == '>' && i < size)
                {
                    c++; i++;
                }
				i--;
                fprintf(out, "\tadd $%i, %%ecx\n", c);
                }
                /* Attempt to optimize useless adds and subtracts */
                else if(procbuf[i+1] == '<')
                    break;
                else
                    fprintf(out, "\tinc %%ecx\n");
                break;

            case '<':
            /* Attempt to optimize repeated decrements */
            if(procbuf[i+1] == '<')
            {
                int c = 0;
                while(procbuf[i] == '<' && i < size)
                {
                    c++; i++;
                }
                fprintf(out, "\tsub $%i, %%ecx\n", c);
            }
            /* Attempt to optimize useless adds and subtracts */
            else if(procbuf[i+1] == '>')
                break;
            else
                fprintf(out, "\tdec %%ecx\n");
            break;

			case '[':
			/* Detect a clearing of a cell and optimize it */
			if(!strncmp(procbuf + i, "[-]", 3))
			{
				fprintf(out, "\tmovb $0, arr(, %%ecx)\n");
				i += 2;
			}
			else
			{
				symcount++;
				symbols = realloc(symbols, symcount * sizeof(char*));
				symbols[symcount-1] = nextsym;
				symnum++;
				nextsym = malloc(snprintf(NULL, 0, ".L%i", symnum));
				sprintf(nextsym, ".L%i", symnum);
				fprintf(out, "\tcmpb $0, arr(, %%ecx)\n\tje %s\n%s:\n", nextsym, symbols[symcount-1]);
				symcount++;
				symbols = realloc(symbols, symcount * sizeof(char*));
				symbols[symcount-1] = nextsym;
				symnum++;
				nextsym = malloc(snprintf(NULL, 0, ".L%i", symnum));
				sprintf(nextsym, ".L%i", symnum);
			}
			break;
			
			case ']':
			fprintf(out, "\tcmpb $0, arr(, %%ecx)\n\tjne %s\n%s:\n", symbols[symcount-2], symbols[symcount-1]);
			symcount -= 2;
			symbols = realloc(symbols, symcount * sizeof(char*));
			break;

			/* TODO: Add syscall optimizations */
			case '.':
			fprintf(out, "%s", getOutStr());
			break;

			case ',':
			fprintf(out, "%s", getInStr());

			default:
			break;
		}
		i++;
	}

	fprintf(out, "%s", getExitStr());
	return;
}
