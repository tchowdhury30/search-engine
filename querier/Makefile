# Makefile for the 'querier' component

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBDIR) -I$(COMMONDIR)

# Paths
LIBDIR = ../libcs50
COMMONDIR = ../common

# Libraries
LLIBS = $(LIBDIR)/libcs50-given.a 

# Source files
# Ensure the path to file.c is corrected if it resides in libcs50
SRC_QUERIER = querier.c $(COMMONDIR)/pagedir.c $(COMMONDIR)/word.c $(COMMONDIR)/index.c $(LIBDIR)/file.c

# Object files
OBJ_QUERIER = $(SRC_QUERIER:.c=.o)

# Executable names
EXEC_QUERIER = querier

.PHONY: all clean querier

# top-level rule to build the program
all: querier

# build the querier program
querier: $(OBJ_QUERIER)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $(EXEC_QUERIER)

# dependencies: object files depend on header files
# Include the correct path for file.h here
$(OBJ_QUERIER) : $(COMMONDIR)/pagedir.h $(COMMONDIR)/word.h $(COMMONDIR)/index.h $(LIBDIR)/webpage.h $(LIBDIR)/file.h

# clean up
clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXEC_QUERIER)
	rm -f $(COMMONDIR)/*~ $(COMMONDIR)/*.o
	rm -f $(LIBDIR)/*~ $(LIBDIR)/*.o

test:
	./testing.sh &> output.txt