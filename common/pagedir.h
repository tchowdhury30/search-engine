/*
 * pagedir.h - header file for the 'pagedir' module.
 *
 * see indexer.h for more information.
 * The 'pagedir' module contains functions that are used by a web crawler to save
 * and load pages to and from a designated directory. This module provides essential
 * functionalities to mark a directory as being used by the crawler, to save web pages
 * into the directory with a unique document ID, and to validate that a directory
 * was created by the crawler. Additionally, it provides a function to load a saved
 * webpage from the directory, reconstructing it into a webpage_t structure.
 *
 * The main functions include:
 * - pagedir_init() to mark a directory for crawler's use.
 * - pagedir_save() to save a webpage's contents to a file within the directory.
 * - pagedir_validate() to check if a directory contains a .crawler file, indicating
 *   it was created by the crawler.
 * - pagedir_load() to load a saved webpage from a file, given its document ID.
 *
 * These functions are instrumental in managing the storage and retrieval of webpages
 * by the crawler and potentially by other components that need to access the crawled
 * data, like a search engine indexer.
 *
 * Tasnim Chowdhury, CS50, February 2024
 */

#ifndef PAGEDIR_H
#define PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"

/*
 * Initializes a directory for use by the crawler.
 * 
 * This function creates a file named '.crawler' within the specified directory
 * to mark it as an output directory of the CS50 crawler. This serves as an indicator
 * to other programs that the directory contains data produced by the crawler.
 *
 * Parameters:
 *  - pageDirectory: A string representing the path to the directory to be initialized.
 *
 * Returns:
 *  - true if the directory was successfully marked as a crawler output directory,
 *    false if the '.crawler' file could not be created (e.g., due to permission issues).
 */

bool pagedir_init(const char* pageDirectory);

/*
 * Saves a webpage's content to a file within a specified directory.
 *
 * This function writes the URL, depth, and HTML content of a webpage to a file
 * named after the webpage's document ID within the specified directory. The format
 * of the saved file is:
 *   First line: URL
 *   Second line: depth
 *   Remaining: HTML content
 *
 * Parameters:
 *  - page: A pointer to the webpage_t structure containing the webpage's data.
 *  - pageDirectory: A string representing the path to the directory where the webpage
 *                   is to be saved.
 *  - docID: An integer representing the document ID assigned to the webpage.
 *
 * Note:
 *  - The function checks for file creation and write errors, reporting them via stderr.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/*
 * Verifies whether a directory was produced by the CS50 crawler.
 *
 * This function checks for the presence of a '.crawler' file within the specified
 * directory to determine if it contains crawler output. The presence of this file
 * is used as an indicator that the directory was initialized by the crawler.
 *
 * Parameters:
 *  - dir: A string representing the path to the directory to be checked.
 *
 * Returns:
 *  - true if the directory contains a '.crawler' file, indicating it was initialized
 *    by the crawler; false otherwise.
 */
bool pagedir_validate(const char* dir);

/*
 * Loads a webpage from a file in a specified directory and returns a webpage_t object.
 *
 * This function constructs the file name using the directory path and document ID,
 * then opens and reads the file to create a new webpage object with the URL, depth,
 * and HTML content found within. The format of the file should match that produced
 * by pagedir_save: URL on the first line, depth on the second, followed by HTML content.
 *
 * Parameters:
 *  - dir: A string representing the path to the directory containing the file.
 *  - docID: An integer representing the document ID of the webpage to load.
 *
 * Returns:
 *  - A pointer to a new webpage_t object containing the loaded data; NULL if the file
 *    could not be opened or the webpage data could not be successfully read.
 * 
 * Note:
 *  - This function dynamically allocates memory for the URL and HTML strings within
 *    the webpage_t object, which must be freed by calling webpage_delete().
 */
webpage_t* pagedir_load(const char* dir, int docID); 

#endif // PAGEDIR_H
