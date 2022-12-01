/*
*       This file is part of bfc.
*
*       bfv is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*       bfc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along with bfc. If not, see <https://www.gnu.org/licenses/>.
*/
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
