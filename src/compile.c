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
#define SYS_write 0
#define SYS_exit 60

#endif

#include "include/args.h"
#include "include/compile.h"
#include "include/syscalls.h"

void compile(uint8_t *procbuf, size_t size, FILE *out)
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
			case '+':
			fprintf(out, "\tincb arr(, %%ecx)\n");
			break;

			case '-':
			fprintf(out, "\tdecb arr(, %%ecx)\n");
			break;

			case '>':
                        fprintf(out, "\tinc %%ecx\n");
                        break;

                        case '<':
                        fprintf(out, "\tdec %%ecx\n");
                        break;

			case '[':
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
			break;
			
			case ']':
			fprintf(out, "\tcmpb $0, arr(, %%ecx)\n\tjne %s\n%s:\n", symbols[symcount-2], symbols[symcount-1]);
			symcount -= 2;
			symbols = realloc(symbols, symcount * sizeof(char*));
			break;

			case '.':
			fprintf(out, getOutStr());
			break;

			case ',':
			fprintf(out, getInStr());

			default:
			break;
		}

		i++;
	}
	fprintf(out, getExitStr());
        return;
}
