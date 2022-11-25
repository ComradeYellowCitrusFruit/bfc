#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "include/compile.h"

char **symbols;
char *nextsym;
int symcount = 0;
int symnum = 0;

void compile_optimized(uint8_t *procbuf, size_t size, FILE *out)
{
	/* Initialize this shit */
	fprintf( \
		".text\n" \
		".globl _start\n" \
		".comm arr, 3000"
		"_start:\n" \
		"\txor %%ecx, %%ecx\n" \
	);

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
				fprintf(out, "\tincb arr(, %%ecx)\n");
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
                                fprintf(out, "\tinc %%ecx\n");
                        break;

			case '[':
			/* Detect a clearing of a cell and optimize it */
			if(!strncmp(procbuf + i, "[-]", 3))
				fprintf(out, "\tmovb $0, arr(, %%ecx)\n");
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
			fprintf(out, "\tmov %i, %%rax\n\tmov $1, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n", SYS_write);
			break;

			case ',':
			fprintf(out, "\tmov %i, %%rax\n\tmov $0, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n", SYS_read);
			break;
			
			default:
			break;	
		}
	}
}
