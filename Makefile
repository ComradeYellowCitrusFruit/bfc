incdir = ./
srcdir = ./src
CFLAGS=-O2 -Wpedantic -Wall

CC=cc

installdir=/usr/bin

.PHONY: all
all: bfc

bfc: $(srcdir)/main.c compile.o optimized.o syscalls.o
	$(CC) $^ $(CFLAGS) -I$(incdir) -o $@

compile.o: $(srcdir)/compile.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c

optimized.o: $(srcdir)/optimized.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c

syscalls.o: $(srcdir)/syscalls.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: install
install:
	cp bfc $(installdir)/bfc
