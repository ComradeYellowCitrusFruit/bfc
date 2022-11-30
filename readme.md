# A simple optimizing brainfuck compiler

bfc (shorthand for Brainfuck Compiler) is a simple optimizing brainfuck compiler.
It produces AT&T syntax assembly code, using system calls for the `,` and `.` instructions.

## Dependencies

All you need to compile bfc is a C compiler and make.

All you need to assemble a program produced by bfc is an AT&T syntax compatible assembler and linker.
On Windows you also need to link with libc since Windows system calls can't be made directly.

## Arguments

- `-h`: prints a help message
- `-O`: enables optimizations
- `-o [FILE]`: sets the output to be sent to FILE
- `-a [FILES]`: appends files to the input file
- `-C[NUM]`: changes the cell count from it's default 3000 to NUM truncated to 24bits
- `-Cf[NUM]`: changes the cell count from it's default 3000 to NUM, no truncation
- `-p`: enables procedure extensions
- `--target-os=[OS]`: sets the target operating system, valid values are host, Win/Win32/NT, linux/gnu-linux/linux-gnu, openbsd, or freebsd, default is host.

## TODOs

- Implement the procedure extensions
