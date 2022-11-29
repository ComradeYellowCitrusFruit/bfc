#include "include/args.h"
#include "include/syscalls.h"

char linux_outcall[] = "\tmov $0, %%rax\n\tmov $1, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n";
char linux_incall[]  = "\tmov $1, %%rax\n\tmov $1, %%rdi\n\tlea arr(, %%ecx), %%rsi\n\tmov $1, %%rdx\n\tpush %%rcx\n\tsyscall\n\tpop %%rcx\n";
char linux_exitcall[] = "\tmov $60, %%rax\n\txor %%rdi, %%rdi\n\tsyscall\n";

/* TODO: BSD system calls */

char win32_outcall[] = "\tpush %%rcx\n\tmovb arr(, %%ecx), %%cl\n\tcall putchar\n\tpop %%rcx\n";
char win32_incall[] = "\tcall getchar\n\tmovb %%al, arr(, %%ecx)\n";

/* On Windows we can use libc and therefore main because we can't directly use system calls. */
char win32_exitcall[] = "\txor %%ecx, %%ecx\n\tret";

/* Get the correct write call string */
char *getOutStr()
{
    if(!args.targ)
        return linux_outcall;
    /*
    else if(args.targ == OPENBSD)
        return openbsd_outcall;
    else if(args.targ == FREEBSD)
        return freebsd_outcall;
    */
    else
        return win32_outcall;
}

/* Get the correct read call string */
char *getInStr()
{
    if(!args.targ)
        return linux_incall;
    /*
    else if(args.targ == OPENBSD)
        return openbsd_incall;
    else if(args.targ == FREEBSD)
        return freebsd_incall;
    */
    else
        return win32_incall;
}

/* Get the correct exit call string */
char *getExitStr()
{
    if(!args.targ)
        return linux_exitcall;
    /*
    else if(args.targ == OPENBSD)
        return openbsd_exitcall;
    else if(args.targ == FREEBSD)
        return freebsd_exitcall;
    */
    else
        return win32_exitcall;
}