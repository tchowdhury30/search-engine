/**
 * indexer.h - Header file for the Tiny Search Engine (TSE) indexer module
 *
 * This module defines the functions used by the TSE indexer to validate command-line arguments,
 * process webpages, and build an index mapping words to document IDs and word counts. It serves
 * as an interface between the main indexer functionality and the utility functions that support
 * the indexing process. The module ensures proper initialization of the indexer's components,
 * facilitates the addition of words from webpage content to the index, and manages the construction
 * of the index from documents stored in a specified directory.
 *
 * Usage:
 *  - parseArgs: Validates and parses command-line arguments for the indexer.
 *  - indexPage: Processes a single webpage, extracting words and adding them to the index.
 *  - indexBuild: Constructs the index by processing all documents within a given directory.
 *
 * CS50, February 2024
 * Tasnim Chowdhury
 */

#ifndef __INDEXER_H
#define __INDEXER_H

#include "../common/index.h"
#include <stdio.h>

/**
 * Validates command-line arguments and initializes function parameters.
 * Ensures exactly two arguments are passed and validates the provided pageDirectory
 * and indexFilename for their respective purposes.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @param pageDirectory Pointer to char* that will be updated with the pageDirectory argument.
 * @param indexFilename Pointer to char* that will be updated with the indexFilename argument.
 * Exits program on failure with appropriate error message.
 */
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);

/**
 * Adds words from a file into the index. Reads the content of a webpage file,
 * normalizes and adds each word into the index with its document ID.
 *
 * @param index The index structure where words will be added.
 * @param page The webpage_t pointer to the webpage being processed.
 * @param docID The document ID associated with the current file.
 * @return True if words were successfully added to the index, false on failure.
 */
bool indexPage(index_t* index, webpage_t* page, int docID);

/**
 * Builds an index from the contents of the page directory. Iterates over each document file
 * in the page directory and processes each file to add words to the index.
 *
 * @param dir The directory containing the crawler-produced files.
 * @return A pointer to the populated index structure, or NULL on failure.
 */
index_t* indexBuild(const char* dir);


#endif // __INDEXER_H
