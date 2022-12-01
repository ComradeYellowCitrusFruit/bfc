/*
*       This file is part of bfc.
*
*       bfv is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*       bfc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along with bfc. If not, see <https://www.gnu.org/licenses/>.
*/


#include "include/args.h"
#include "include/syscalls.h"

char linux_outcall[] = "\tmov $0, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $1, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char linux_incall[]  = "\tmov $1, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $1, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char linux_exitcall[] = "\tmov $60, %rax\n\txor %rdi, %rdi\n\tsyscall\n";

char openbsd_outcall[] = "\tmov $4, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $2, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char openbsd_incall[] = "\tmov $3, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $1, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char openbsd_exitcall[] = "\tmov $1, %rax\n\txor %rdi, %rdi\n\tsyscall\n";

char freebsd_outcall[] = "\tmov $4, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $2, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char freebsd_incall[] = "\tmov $3, %rax\n\tmov $1, %rdi\n\tlea arr(, %ecx), %rsi\n\tmov $1, %rdx\n\tpush %rcx\n\tsyscall\n\tpop %rcx\n";
char freebsd_exitcall[] = "\tmov $1, %rax\n\txor %rdi, %rdi\n\tsyscall\n";

/* On Windows we can use libc and therefore main because we can't directly use system calls. */
char win32_outcall[] = "\tpush %rcx\n\tmovb arr(, %ecx), %cl\n\tcall putchar\n\tpop %rcx\n";
char win32_incall[] = "\tcall getchar\n\tmovb %al, arr(, %ecx)\n";
char win32_exitcall[] = "\txor %rax, %rax\n\tret";

/* Get the correct write call string */
char *getOutStr()
{
	if(!args.targ)
		return linux_outcall;
	else if(args.targ == OS_OPENBSD)
		return openbsd_outcall;
	else if(args.targ == OS_FREEBSD)
		return freebsd_outcall;
	else
		return win32_outcall;
}

/* Get the correct read call string */
char *getInStr()
{
	if(!args.targ)
		return linux_incall;
	else if(args.targ == OS_OPENBSD)
		return openbsd_incall;
	else if(args.targ == OS_FREEBSD)
		return freebsd_incall;
	else
		return win32_incall;
}

/* Get the correct exit call string */
char *getExitStr()
{
	if(!args.targ)
		return linux_exitcall;
	else if(args.targ == OS_OPENBSD)
		return openbsd_exitcall;
	else if(args.targ == OS_FREEBSD)
		return freebsd_exitcall;
	else
		return win32_exitcall;
}
