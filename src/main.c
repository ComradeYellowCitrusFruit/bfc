#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef _WIN32

#include <sys/mman.h>
#include <unistd.h>

#else

#include <Windows.h>

#endif

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
		"--target-os=[OS] - Sets the target operating system, default is host, valid values are linux/gnu-linux/linux-gnu, Win/Win32/NT, openbsd, freebsd, or host\n"
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
#ifndef _WIN32
	char procs[] = "/tmp/BFC_PROC_XXXXXX";
	int procfd = mkstemp(procs);
#else
	char procs[MAX_PATH];
	GetTempPath(MAX_PATH, procs);
	HANDLE procfd = CreateFileA(procs, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE);
#endif
	FILE *iF = NULL;
	bool appending = false;
	char *idir = NULL;
	args.outfile = NULL;
	args.targ = HOST;

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
#ifndef _WIN32
				write(procfd, &C, 1);
#else
				DWORD junk;
				WriteFile(procfd, &C, 1, &junk, NULL);
#endif
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
#ifndef _WIN32
				write(procfd, &C, 1);
#else
				DWORD junk;
				WriteFile(procfd, &C, 1, &junk, NULL);
#endif
			}
			fclose(iF);

			if(idir != NULL)
			{
				free(name);
			}
		}
		else if(!strncmp(argv[i], "--target-os", strlen("--target-os")))
		{
			int j = 0;

			while(argv[i][j] != '=' && argv[i][j]) 
				j++;

			if(!argv[i][j])
				printf("Invalid use of --target-os - '%s'\n", argv[i]);
			else
			{
				char *osstr = argv[i] + j;
				if(!strcmp(osstr, "linux") || !strcmp(osstr, "linux-gnu") || !strcmp(osstr, "gnu-linux"))
					args.targ = LINUX;
				else if(!strcmp(osstr, "Win") || !strcmp(osstr, "Win32") || !strcmp(osstr, "NT"))
					args.targ = WIN32;
				else if(!strcmp(osstr, "openbsd"))
					args.targ = OPENBSD;
				else if(!strcmp(osstr, "freebsd"))
					args.targ = FREEBSD;
				else if(!strcmp(osstr, "host"))
					args.targ = HOST;
				else
				{
					printf("Invalid value of [OS] in --target-os=[OS]: '%s'", osstr);
				}
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

	if(args.targ == WIN32)
		printf( \
			"In order to successfully create a final executable from this output, you will need to link with your libc.\n" \
			"Output assembly will be in AT&T/GNU as syntax, which you may need to convert to your assembler's syntax.\n" \
		);

#ifndef _WIN32
	/* Map procfd into memory */
	size_t procsize = lseek(procfd, 0, SEEK_END);
	void *procptr = mmap(NULL, procsize, PROT_WRITE | PROT_READ, MAP_PRIVATE, procfd, 0);
#else 
	/* What the other comment says */
	DWORD procsize = GetFileSize(procfd, NULL);
	HANDLE mapping = CreateFileMappingA(procfd, NULL, PAGE_READWRITE, 0, procsize, NULL);
	void *procptr = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, procsize);
#endif

	/* Open the output file and get moving */
	FILE *of = fopen(args.outfile, "w+");
	if(args.optimize)
		compile_optimized(procptr, procsize, of);
	else
		compile(procptr, procsize, of);
	
	/* Clean up time */
#ifndef _WIN32
	munmap(procptr, procsize);
	close(procfd);
	unlink(procs);
#else
	UnmapViewOfFile(procptr);
	CloseHandle(mapping);
#endif
	return 0;
}
