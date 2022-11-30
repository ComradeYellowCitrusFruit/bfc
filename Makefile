incdir = ./
srcdir = ./src
CFLAGS=-O2 -Wpedantic -Wall

CC=cc

.PHONY: all
all: bfc

bfc: $(srcdir)/main.c compile.o optimized.o
	$(CC) $^ $(CFLAGS) -I$(incdir) -o $@

compile.o: $(srcdir)/compile.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c

optimized.o: $(srcdir)/optimized.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: install
install:
	cp bfc /usr/local/bfc
