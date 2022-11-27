#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "include/args.h"
#include "include/compile.h"

Args_state_t args;

static void print_help()
{
	printf(
		"Usage: bfc infile [-O] [-a [FILES]] [-Idir] [-o [FILE]]\n" \
		"-O - Enables optimizations\n" \
		"-a [FILES] - Appends [FILES] to the end of the infile, most useful with -Idir, will be appended even if no infile has been added yet\n" \
		"-Idir - Sets dir to be the search path for files\n" \
		"-o [FILE] - Sets the output file, default a.s\n" \
	);
}

int main(int argc, char **argv)
{
	/* Handle args */
	if(argc < 2)
	{
		printf("Usage: bfc infile [-O] [-a [FILES] [-o [FILE]]\n");
		return -1;
	}

	if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
	{
		print_help();
		return 0;
	}

	/* Propagate this shit */
	char procs[] = "/tmp/BFC_PROC_XXXXXX";
	int procfd = mkstemp(procs);
	FILE *iF = NULL;
	bool appending = false;
	char *idir = NULL;
	args.outfile = NULL;
	for(int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			print_help();
			return 0;
		}
		else if(!strcmp(argv[i], "-O"))
		{
			args.optimize = true;
			appending = false;
		}
		else if(!strcmp(argv[i], "-a"))
			appending = true;
		else if(!strncmp(argv[i], "-I", 2))
		{
			idir = argv[i] + 2;
			appending = false;
		}
		else if(!strcmp(argv[i], "-o"))
		{
			i++;
			if(i < argc)
				args.outfile = argv[i];
			else
				args.outfile = "a.s";
		}
		else if(appending)
		{
			char *name;
			if(idir != NULL)
			{
				name = malloc(strlen(argv[i]) + strlen(idir) + 1);
				strcpy(name, idir);
				strcat(name, argv[i]);
			}
			else
				name = argv[i];

			FILE *afile = fopen(name, "r");
			int c;
			char C;
			while((c = fgetc(afile)) != EOF)
			{
				C = (char)c;
				write(procfd, &C, 1);
			}
			fclose(afile);
			if(idir != NULL)
				free(name);
		}
		else if(iF == NULL)
		{
			char *name;
			if(idir != NULL)
			{
				name = malloc(strlen(argv[i]) + strlen(idir) + 1);
				strcpy(name, idir);
				strcat(name, argv[i]);
			}
			else
				name = argv[i];

			iF = fopen(name, "r");
			int c;
			char C;
			while((c = fgetc(iF)) != EOF)
			{
				C = (char)c;
				write(procfd, &C, 1);
			}
			fclose(iF);

			if(idir != NULL)
			{
				free(name);
			}
		}
		else
			fprintf(stderr, "Invalid argument: %s\n", argv[i]);
	}

	/* Make sure we have enough args */
	if(iF == NULL)
	{
		fprintf(stderr, "Invalid arguments, no infile.\n");
		return -1;	
	}
	else if(args.outfile == NULL)
		args.outfile = "a.s";

	/* Map procfd into memory */
	size_t procsize = lseek(procfd, 0, SEEK_END);
	/* Add ten bytes as padding to prevent segfaults */
	void *procptr = mmap(NULL, procsize + 10, PROT_WRITE | PROT_READ, MAP_PRIVATE, procfd, 0);

	/* Open the output file and get moving */
	FILE *of = fopen(args.outfile, "w+");
	if(args.optimize)
		compile_optimized(procptr, procsize, of);
	else
		compile(procptr, procsize, of);
	
	/* Clean up time */
	munmap(procptr, procsize);
	close(procfd);
	unlink(procs);
	return 0;
}
