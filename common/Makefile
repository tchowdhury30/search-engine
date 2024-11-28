# object files, and the target library
OBJS = pagedir.o index.o word.o

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

# Target library
LIB = libcommon.a

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Compile pagedir.c into pagedir.o
pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c -o pagedir.o

# Compile index.c into index.o
index.o: index.c index.h ../libcs50/hashtable.h ../libcs50/counters.h
	$(CC) $(CFLAGS) -c index.c -o index.o

# Compile word.c into word.o
word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c -o word.o

.PHONY: clean

# Clean up
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
