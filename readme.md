# A simple optimizing brainfuck compiler

bfc (shorthand for Brainfuck Compiler) is a simple optimizing brainfuck compiler.
It produces assembly code, using system calls for the `,` and `.` instructions.
At the moment, it does not support Windows, only \*nix systems.
Produced code only supports amd64 linux at the moment.

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

- Implement changing the cell count
- Implement the procedure extensions
- Implement changes to target OS
