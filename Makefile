incdir = ./
srcdir = ./src
CFLAGS=-O2 -ansi -wpedantic -wall -wextra

.PHONY: all
all: bfc

bfc: main.o compile.o
	$(CC) $^ $(CFLAGS) -I$(incdir) -o $@

main.o: $(srcdir)/main.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c -o $@

compile.o: $(srcdir)/compile.c $(srcdir)/optimized.c
	$(CC) $^ $(CFLAGS) -I$(incdir) -c -o $@

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: install
install:
	cp bfc /usr/local/bfc
