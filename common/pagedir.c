/*
 * pagedir.c - CS50 'pagedir' module
 *
 * pagedir utilities - initalizing, saving, validating, loading
 *
 * Tasnim Chowdhury, 2/4/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include <dirent.h>
#define _POSIX_C_SOURCE 200809L


const int pathLength = 256;


bool pagedir_init(const char* pageDirectory) {
    char filePath[pathLength]; 

    // Construct the pathname for the .crawler file in the directory
    snprintf(filePath, sizeof(filePath), "%s/.crawler", pageDirectory);

    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        return false; // Return false if file creation fails
    }
    fclose(file); // Close the file after creating it
    return true; // Return true indicating successful initialization
}


void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char filePath[pathLength]; 

    // Construct the pathname for the page file in pageDirectory
    snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, docID);
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open page file.\n");
        exit(1); // Exit if file cannot be opened for writing
    }
    // Write the URL, depth, and HTML content to the file
    if (fprintf(file, "URL %s\n", webpage_getURL(page)) < 0 ||
        fprintf(file, "depth %d\n", webpage_getDepth(page)) < 0 ||
        fprintf(file, "web contents %s", webpage_getHTML(page)) < 0) {
        perror("Error writing to file"); // More informative error
        fprintf(stderr, "Failed to write to file: %s\n", filePath);
    }

    fflush(file); // Flush the stream to ensure data is written to the file
    fclose(file); // Close the file after writing
    return;
}


bool pagedir_validate(const char* dir) 
{
    // Attempt to open the directory for reading
    DIR *directory = opendir(dir);
    struct dirent *entry;
    bool found = false; // Flag to indicate if the .crawler file is found
    
    if (directory != NULL) {
        // Loop through each entry in the directory
        while ((entry = readdir(directory)) != NULL) {
            // Check if the current entry is the .crawler file
            if (strcmp(entry->d_name, ".crawler") == 0) {
                found = true; // Mark as found and exit loop
                break;
            }
        }
        // Clean up: close the directory after checking all entries
        closedir(directory);
    } 
    // Return true if .crawler file is found, indicating crawler initialization
    return found;
}


webpage_t* pagedir_load(const char* dir, int docID) {
    // Construct the full pathname for the file corresponding to the given docID
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%d", dir, docID);

    // Attempt to open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        //fprintf(stderr, "Could not open file %s\n", filename);
        return NULL; // Early return if the file cannot be opened
    }

    // Read the URL from the first line of the file
    char* url = file_readLine(file);
    if (url == NULL) {
        fclose(file); // Clean up before returning NULL
        return NULL;
    }

    // Read the depth from the next line of the file
    char depthStr[256];
    if (fgets(depthStr, sizeof(depthStr), file) == NULL) {
        free(url); // Free dynamically allocated URL string
        fclose(file); // Clean up before returning NULL
        return NULL;
    }
    int depth = atoi(depthStr); // Convert depth string to integer

    // Read the remaining file content as HTML
    char* html = file_readFile(file);
    if (html == NULL) {
        free(url); // Free dynamically allocated URL string
        fclose(file); // Clean up before returning NULL
        return NULL;
    }

    fclose(file); // Close the file after successful reading

    // Create a new webpage object with the read data
    webpage_t* page = webpage_new(url, depth, html);
    // Note: webpage_new takes ownership of url and html memory

    return page; // Return the newly created webpage object

}
