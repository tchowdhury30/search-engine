/*
 * indextester.c - CS50 'indexetester' module
 *
 * Usage: ./indextest oldIndexFilename newIndexFilename
 * where oldIndexFilename is the name of a file produced by the indexer
 * where newIndexFilename is the name of a file into which the index should be written.
 * 
 * Tasnim Chowdhury, 2/4/24
 * 
 * make indextest; ./indextest ../data/crawldata/letters-0/.index  ../data/crawldata/letters-0/poopp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "indexer.h"

int main(const int argc, char* argv[]) {
     if (argc != 3) {
        fprintf(stderr, "Usage: %s oldIndexFilename newIndexFilename\n", argv[0]);
        exit(1);
    }
    index_t *index = index_new();
    fileToIndex(index, argv[1]);
    indexToFile(index, argv[2]); 
    exit(0);
} 
