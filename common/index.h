/* 
 * index.h - header file for the CS50 'index' module
 *
 * The 'index' module defines a data structure for efficiently storing
 * and accessing a word index. The index maps words to document IDs and
 * the frequency of occurrences within those documents. This is implemented
 * using a hashtable, where each key is a word, and its value is a
 * counters set tracking the document IDs and occurrence counts.
 *
 * This module supports creating a new index, adding words with document
 * IDs, finding the occurrence count of words, iterating over index items,
 * and saving/loading the index from/to a file.
 *
 * The index is crucial for search engine operations, enabling efficient
 * keyword searches through documents.
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 * Updated by Tasnim Chowdhury, February 2024
 *
 * Compilation requires: 
 * - libcs50 (hashtable.h, counters.h)
 */

#ifndef __INDEX_H
#define __INDEX_H

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/* 
 * Global variables
 */
#define NUM_SLOTS 200  // Default number of slots in hashtable

/* 
 * Struct definitions
 */
typedef struct index {
    hashtable_t *ht;  // Hashtable: words as keys, counters as values
} index_t;

/* 
 * index_new: Creates and initializes a new, empty index.
 *
 * Allocates memory for an index structure and initializes a hashtable
 * with a predefined number of slots for storing word occurrences.
 *
 * Returns:
 *   - A pointer to the new index, or NULL if an error occurs.
 */
index_t *index_new(void);

/*
 * index_delete - frees the memory allocated for an index and its contents.
 *
 * Deletes the hashtable within the index, freeing all associated memory,
 * and then frees the memory allocated for the index structure itself.
 *
 * Parameters:
 *  - index: a pointer to the index to be deleted.
 */
void index_delete(index_t *index);

/*
 * index_find - retrieves the counter set associated with a given word.
 *
 * Searches the index's hashtable for a counter set corresponding to the given word.
 *
 * Parameters:
 *  - index: a pointer to the index being searched.
 *  - word: the word for which to find the counter set.
 *
 * Returns a pointer to the counter set associated with the word, or NULL if
 * the word is not found in the index.
 */
counters_t *index_find(index_t *index, const char *word);

/*
 * index_add - adds a word occurrence to the index.
 *
 * If the word is already in the index, increments the count for the given docID.
 * If the word is not in the index, adds it with the given docID and a count of 1.
 *
 * Parameters:
 *  - index: a pointer to the index to which the word occurrence should be added.
 *  - word: the word to add.
 *  - docID: the document ID where the word was found.
 *
 * Returns true if the word occurrence was successfully added, false otherwise.
 */
bool index_add(index_t *index, const char *word, int docID);

/*
 * index_iterate - iterates over all items in the index.
 *
 * Calls the given function for each item (word and its counter set) in the index.
 *
 * Parameters:
 *  - index: a pointer to the index to be iterated over.
 *  - arg: a pointer to any additional arguments for the itemfunc.
 *  - itemfunc: a pointer to the function to be called for each item.
 */
void index_iterate(index_t *index, void *arg, void (*itemfunc)(void *arg, const char *key, void *item));

/*
 * indexToFile - writes the entire contents of an index to a file.
 *
 * Iterates over each entry in the index and writes it to the specified file.
 *
 * Parameters:
 *  - index: a pointer to the index to be written.
 *  - filename: the name of the file to which the index should be written.
 */
void indexToFile(index_t *index, const char *filename);

/*
 * counterToFile - writes a single document ID and count to a file.
 *
 * Designed to be called by counters_iterate for each item in a counter set.
 *
 * Parameters:
 *  - arg: a pointer to the file to which the data should be written.
 *  - key: the document ID.
 *  - count: the count of occurrences for the document ID.
 */
void counterToFile(void *arg, const int key, const int count);

/*
 * entryToFile - writes a single index entry (word and its counter set) to a file.
 *
 * Designed to be called by hashtable_iterate for each item in the index.
 *
 * Parameters:
 *  - arg: a pointer to the file to which the data should be written.
 *  - key: the word.
 *  - item: a pointer to the counter set associated with the word.
 */
void entryToFile(void *arg, const char *key, void *item);

/*
 * fileToIndex - loads an index from a file
 *
 * This function opens and reads an index from a given file ('oldf'), reconstructing
 * the index structure in memory by parsing the word-documentID-count triples contained
 * within. The loaded index can then be manipulated or queried as required by the application.
 * 
 * Parameters:
 *  - index: a pointer to a new index the to load the original index into
 *  - newf: a pointer to a string containing the path to the file to which the index should be written.
 *          If NULL, the index is not written to a new file.
 *
 * Returns:
 *  - A pointer to the newly loaded index structure, or NULL if loading failed due to file access errors
 *    or memory allocation issues.
 *
 * Note:
 *  - It is the caller's responsibility to manage the memory for the returned index structure,
 *    including calling index_delete() when the index is no longer needed.
 */
index_t* fileToIndex(index_t* index, char* newf);

#endif // __INDEX_H
