# Makefile for the 'indexer' component

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBDIR) -I$(COMMONDIR)

# Paths
LIBDIR = ../libcs50
COMMONDIR = ../common

# Libraries
LLIBS = $(LIBDIR)/libcs50-given.a 

# Source files
SRC_INDEXER = indexer.c $(COMMONDIR)/pagedir.c $(COMMONDIR)/word.c $(COMMONDIR)/index.c
SRC_INDEXTEST = indextest.c $(COMMONDIR)/word.c $(COMMONDIR)/index.c

# Object files
OBJ_INDEXER = $(SRC_INDEXER:.c=.o)
OBJ_INDEXTEST = $(SRC_INDEXTEST:.c=.o)

# Executable names
EXEC_INDEXER = indexer
EXEC_INDEXTEST = indextest

.PHONY: all clean test indexer indextest

# top-level rule to build both programs
all: indexer indextest

# build the indexer program
indexer: $(OBJ_INDEXER)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $(EXEC_INDEXER)

# build the indextest program
indextest: $(OBJ_INDEXTEST)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $(EXEC_INDEXTEST)

# dependencies: object files depend on header files
$(OBJ_INDEXER) : $(COMMONDIR)/pagedir.h $(LIBDIR)/webpage.h $(COMMONDIR)/word.h $(COMMONDIR)/index.h
$(OBJ_INDEXTEST) : $(COMMONDIR)/pagedir.h $(LIBDIR)/webpage.h $(COMMONDIR)/word.h $(COMMONDIR)/index.h

# clean up
clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXEC_INDEXER) $(EXEC_INDEXTEST)
	rm -f $(COMMONDIR)/*~ $(COMMONDIR)/*.o
	rm -f $(LIBDIR)/*~ $(LIBDIR)/*.o
	find ../data/crawldata/ -type f \( -name "*.index" -o -name "*.index2" \) -exec rm -f {} +

# testing 
test:
	./testing.sh &> testing.out
	rm readonly.index unwritable.index
	
