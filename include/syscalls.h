#ifndef BRAINFUCK_COMPILER_SYSCALLS_H__
#define BRAINFUCK_COMPILER_SYSCALLS_H__

extern char linux_outcall[];
extern char linux_incall[];
extern char linux_exitcall[];

extern char openbsd_outcall[];
extern char openbsd_incall[];
extern char openbsd_exitcall[];

extern char freebsd_outcall[];
extern char freebsd_incall[];
extern char freebsd_exitcall[];

extern char win32_outcall[];
extern char win32_incall[];
extern char win32_exitcall[];

/* Get the correct write call string */
char *getOutStr();

/* Get the correct read call string */
char *getInStr();

/* Get the correct exit call string */
char *getExitStr();

#endif
