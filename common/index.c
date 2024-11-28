/*
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Tasnim Chowdhury, 2/4/24
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"

typedef struct index {
    hashtable_t *ht; 
} index_t;

const int num_slots = 200; 
const int MaxWordLength = 100;

index_t *index_new() 
{
    if (num_slots > 0) {
        // Allocate memory for index
        index_t *index = mem_malloc(sizeof(index_t));
        if (index == NULL) {
            return NULL;
        }

        index->ht = hashtable_new(num_slots);

        // Make sure memory was properly allocated
        if (index == NULL ) { return NULL; }
        if (index->ht == NULL) {
            mem_free(index);
            return NULL;
        }
        return index;

    } else {
        return NULL; // If invalid table length
    }
}

/*
 * itemdelete_wrapper - a wrapper function for deleting counter sets.
 *
 * Designed to be passed to hashtable_delete as a parameter to specify how
 * to deal with the items (counters sets) stored in the hashtable.
 *
 * Parameters:
 *  - item: a pointer to the item (counters set) to be deleted.
 */
static void itemdelete_wrapper(void *item) {
    counters_delete((counters_t *)item);
}


void index_delete(index_t *index) {
    if (index != NULL) {
        hashtable_delete(index->ht, itemdelete_wrapper);  
        free(index); // Don't forget to free the index itself after deleting its contents
    }
}

counters_t *index_find(index_t *index, const char *word) {
    if (index != NULL && word != NULL) {
        // Use hashtable_find to retrieve the counters set for the word
        return (counters_t *)hashtable_find(index->ht, word);
    }
    return NULL; // Return NULL if index or word is NULL
}

bool index_add(index_t *index, const char *word, int docID) 
{
    if (index != NULL && docID >= 0) {
        counters_t *wordInfo = hashtable_find(index->ht, word); // Attempt to find the word in the hashtable
        if (wordInfo != NULL) { // If the word is already in the index
            if (counters_add(wordInfo, docID) > 0) { return true; } // Increment the count for the docID
        } else { // If the word is not in the index
            wordInfo = counters_new(); // Create a new counter set for the word
            if (wordInfo != NULL && counters_add(wordInfo, docID) > 0) { // Initialize the counter set
                if (hashtable_insert(index->ht, word, wordInfo)) { return true; } // Add the new word to the hashtable
                else { counters_delete(wordInfo); } // Clean up if insertion fails
            }
        }
    }
    return false; // Return false if adding failed
}


void index_iterate(index_t *index, void *arg,
void (*itemfunc)(void *arg, const char *key, void *item) ) {
    if (index == NULL || itemfunc == NULL) {
        return;
    }

    hashtable_iterate(index->ht, arg, itemfunc);
}


void counterToFile(void *arg, const int key, const int count) {
    FILE *fp = (FILE *)arg;
    fprintf(fp, " %d %d", key, count);  // Write docID and count
}


void entryToFile(void *arg, const char *key, void *item) {
    FILE *fp = (FILE *)arg;
    counters_t *counters = (counters_t *)item;

    fprintf(fp, "%s", key);  // Write the word
    counters_iterate(counters, fp, counterToFile);  // Write its counters
    fprintf(fp, "\n");  // New line for next word
}


void indexToFile(index_t *index, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    index_iterate(index, fp, entryToFile);
    fclose(fp);
}

//take in an index file in the correct format and convert it into an index structure 
index_t* fileToIndex(index_t* index, char* oldf) {
    FILE *old = fopen(oldf, "r");
    if (old == NULL) {
        fprintf(stderr, "oldIndexFilename is not a valid file path for reading.\n");
        return NULL;
    }

    char word[MaxWordLength]; 
    int docID = 0;
    int count = 0;

    while (fscanf(old, "%s", word) == 1) {
        // Read docID and count until the end of line
        while (fscanf(old, "%d %d", &docID, &count) == 2) {
            counters_t *wordInfo = index_find(index, word); // Find word in index
            if (wordInfo == NULL) { // If word doesn't exist in index
                wordInfo = counters_new(); // Create a new counter set
                if (wordInfo != NULL) {
                    counters_set(wordInfo, docID, count); // Set the count for the docID
                    hashtable_insert(index->ht, word, wordInfo); // Insert word into index
                }
            } else { // If word exists in index
                counters_set(wordInfo, docID, count); // Update the count for the docID
            }
        }
    } 

    fclose(old);
    return index;
}



