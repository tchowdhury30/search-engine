/*
 * indexer.c - CS50 'indexer' module
 *
 * see indexer.h for more information.
 * Usage: ./indexer pageDirectory indexFilename
 * reads the document files produced by the TSE crawler from pageDirectory dir, builds an index, 
 * and writes that index to a file named indexFilename
 *
 * Tasnim Chowdhury, 2/4/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../libcs50/file.h"
#include "../common/word.h"
#include "indexer.h"

//given arguments from command like, validate them into function parameters
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
        exit(1);
    }

    // Validate pageDirectory by checking if it is produced by the Crawler.
    if (!pagedir_validate(argv[1])) {
        fprintf(stderr, "pageDirectory is not a directory produced by the Crawler.\n");
        exit(1);
    }

    // Check if we can create or overwrite the file at indexFilename.
    FILE *file = fopen(argv[2], "w");
    if (file == NULL) {
        fprintf(stderr, "indexFilename is not a valid file path for writing.\n");
        exit(1);
    }
    fclose(file);

    // Allocate memory and copy the pageDirectory argument.
    *pageDirectory = malloc(strlen(argv[1]) + 1); // +1 for the null terminator
    if (*pageDirectory == NULL) {
        fprintf(stderr, "Failed to allocate memory for pageDirectory.\n");
        exit(1);
    }
    strcpy(*pageDirectory, argv[1]);

    // Allocate memory and copy the indexFilename argument.
    *indexFilename = malloc(strlen(argv[2]) + 1); // +1 for the null terminator
    if (*indexFilename == NULL) {
        fprintf(stderr, "Failed to allocate memory for indexFilename.\n");
        free(*pageDirectory); // Clean up already allocated memory
        exit(1);
    }
    strcpy(*indexFilename, argv[2]);
}

//add words from the file into the index
bool indexPage(index_t* index, webpage_t* page, int docID) 
{
    int pos = 0;
    char* word;

    // Iterate through each word in the page content
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        
        if (strlen(word) >= 3) {  // Only consider words with 3 or more characters
            normalize(word);  // Convert the word to lowercase (or your normalization)
            
            if (!index_add(index, word, docID)) {
                // Handle failure to add the word to the index
                fprintf(stderr, "Failed to add word to index: %s\n", word);
            }
        }
        mem_free(word);  // Don't forget to free the word after processing
    }

    webpage_delete(page);  // Clean up
    return true;
} 


//read the documents contents from page directory 
index_t* indexBuild(const char* dir)
{
    index_t *index = index_new();
    if (index == NULL) {
        return NULL;
    }
    int docID = 1;
    webpage_t* page = NULL;
    while ((page = pagedir_load(dir, docID)) != NULL) {
        if (!indexPage(index, page, docID)) { 
            //issue adding all words from this file to index
            index_delete(index);
            webpage_delete(page);
            return NULL;
        }
        docID++;
        
    }
    return index;

}

int main(const int argc, char* argv[]) {
    char *pageDirectory = NULL;
    char *indexFilename = NULL;

    // Parse command-line arguments and allocate memory for pageDirectory and indexFilename
    parseArgs(argc, argv, &pageDirectory, &indexFilename);

    // Build the index using the validated and stored pageDirectory
    index_t *index = indexBuild(pageDirectory);
    if (index == NULL) {
        fprintf(stderr, "Failed to build index.\n");
        // Free allocated memory before exiting
        free(pageDirectory);
        free(indexFilename);
        exit(1);
    }

    // Write the index to the file specified by indexFilename
    indexToFile(index, indexFilename);

    // Clean up: delete the index and free dynamically allocated memory
    index_delete(index);
    free(pageDirectory);
    free(indexFilename);

    // Exit successfully
    exit(0);
}
