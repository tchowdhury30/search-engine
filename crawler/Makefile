# Makefile for the 'crawler' component

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

# Paths
LIBDIR = ../libcs50
COMMONDIR = ../common

# Libraries
LLIBS = $(LIBDIR)/libcs50-given.a

# Source files
SRCS = crawler.c $(COMMONDIR)/pagedir.c 
OBJS = $(SRCS:.c=.o)

# Executable
PROG = crawler

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

# Dependencies: object files depend on header files
crawler.o: $(COMMONDIR)/pagedir.h
pagedir.o: $(COMMONDIR)/pagedir.h $(LIBDIR)/webpage.h
webpage.o: $(LIBDIR)/webpage.h

# ... (other dependencies)

# clean up after our compilation
clean:
	rm -f core
	rm -f $(PROG) *~ *.o
	rm -f $(COMMONDIR)/*~ $(COMMONDIR)/*.o
	rm -f $(LIBDIR)/*~ $(LIBDIR)/*.o

# Rule for testing
test:
	./testing.sh