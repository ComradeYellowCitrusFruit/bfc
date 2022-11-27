#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "include/args.h"
#include "include/compile.h"

static bool validateChar(char c)
{
	return (c != ' ' || c != '\t' || c != '\n') &&
	(c != '+' || c != '-' || c != '>' || c != '<' || c != '.' || c != ',' || c != '|' || c != '~');
}

void compile(uint8_t *procbuf, size_t size, FILE *out)
{
	/* Initialize this shit */
	fprintf(out, \
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
			case '+':
			fprintf(out, "\tincb arr(, %%ecx)\n");
			break;

			case '-':
			fprintf(out, "\tincb arr(, %%ecx)\n");
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
			fprintf(out, "\tmov %i, %%rax\n\tmov $1, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n", SYS_write);
			break;

			case ',':
			fprintf(out, "\tmov %i, %%rax\n\tmov $0, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n", SYS_read);
			break;
			
			case '~':
			/*	Using regexes would be a good idea, it's a shame we aren't doing it
			*	Python regexes would be especially useful
			*/
			if(!args.gotos)
				break;
			
			i++;
			int lsize = 0;
			char *label = NULL;
			while((i < size && validateChar(procbuf[i])) && procbuf[i] != ':')
			{
				lsize++;
				label = realloc(label, lsize);
				label[lsize-1] = procbuf[i];
				i++;
			}
			lsize++;
			if(!validateChar(procbuf[i]))
				free(label);
			else
			{
				label = realloc(label, lsize);
				label[lsize-1] = '\0';
				fprintf(out, ".%s:\n", label);
				free(label);
			}

			break;

			case '|':
			if(!args.gotos)
				break;

			i++;
			lsize = 0;
			label = NULL;
			while(i < size && validateChar(procbuf[i]))
			{
				lsize++;
				label = realloc(label, lsize);
				label[lsize-1] = procbuf[i];
				i++;
			}
			lsize++;
			label = realloc(label, lsize);
			label[lsize-1] = '\0';
			fprintf(out, "\tjmp %s\n", label);


			default:
			break;
		}

		i++;
	}
	fprintf(out, "\tmov $%i, %%rax\n\txor %%rdi, %%rdi\n\tsyscall\n", SYS_exit);
        return;
}
