incdir = ./
subincdir = ./sys
srcdir = ./src
CFLAGS=-O2 -Wpedantic -Wall

CC=cc

installdir=/usr/bin

.PHONY: all
all: bfc

bfc: $(srcdir)/main.c compile.o optimized.o syscalls.o
	$(CC) $^ $(CFLAGS) -I$(incdir) -idirafter $(subincdir) -o $@

compile.o: $(srcdir)/compile.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -idirafter $(subincdir) -c

optimized.o: $(srcdir)/optimized.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -idirafter $(subincdir) -c

syscalls.o: $(srcdir)/syscalls.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -idirafter $(subincdir) -c

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: install
install:
	cp bfc $(installdir)/bfc
