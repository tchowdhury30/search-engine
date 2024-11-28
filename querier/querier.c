/*
 * querier.c - CS50 'querier' module see querier.h for more details 
 * The TSE Querier reads the index file produced by the TSE Indexer, and page
 * files produced by the TSE Crawler, and answers search queries submitted via stdin.
 * 
 * Usage: ./querier pageDirectory indexFilename
 * where pageDirectory is the pathname of a directory produced by the Crawler, and
 * where indexFilename is the pathname of a file produced by the Indexer.
 * 
 * Tasnim Chowdhury, 2/8/24
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "querier.h"

const int MAX_WORDS = 20;
const int WORD_LENGTH = 10;

typedef struct duo {
    counters_t* first;
    counters_t* second;
} duo_t;


// Parses and validates command line arguments for pageDirectory and indexFilename.
// Expects exactly two arguments (excluding the program name), copies them for further use,
// and validates the pageDirectory to ensure it was created by the Crawler.
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
    if (argc != 3) { // Ensure exactly two arguments are provided.
        fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
        exit(1); // Exit with error if incorrect number of arguments.
    }

    // Duplicate arguments to ensure memory management and modification safety.
    *pageDirectory = strdup(argv[1]);
    *indexFilename = strdup(argv[2]);

    // Validate the pageDirectory to confirm it's a crawler-produced directory.
    if (!pagedir_validate(*pageDirectory)) {
        fprintf(stderr, "pageDirectory %s is not a directory produced by the Crawler.\n", *pageDirectory);
        free(*pageDirectory); // Clean up allocated memory to avoid leaks.
        free(*indexFilename);
        exit(1); // Exit with error if validation fails.
    }
}

// Validates the syntax of a given query, ensuring it contains only letters and spaces.
// This is a preliminary check before further processing and tokenization.
static bool isValidQuery(char *query) {
    for (int i = 0; query[i] != '\0'; i++) {
        if (!isalpha(query[i]) && !isspace(query[i])) {
            return false; // Return false if any character is not a letter or space.
        }
    }
    return true; // Query is valid if it passes the check.
}


void tokenize(char* query, char* words[], int* numWords, bool* isValid) {
    *numWords = 0; // Initialize word count
    *isValid = true; // Assume query is valid initially
    bool inWord = false; // Flag to track if we are currently in a word
    char* lastWord = NULL; // Keep track of the last word encountered

    for (char* cursor = query; *cursor != '\0'; cursor++) {
        // Convert character to lowercase
        *cursor = tolower((unsigned char)*cursor);

        // Check if the current character is part of a word
        if (isalpha((unsigned char)*cursor)) {
            if (!inWord) {
                // Starting a new word
                inWord = true;
                if (*numWords < MAX_WORDS) {
                    words[*numWords] = cursor; // Save the word's start
                    (*numWords)++;
                    lastWord = cursor; // Update last word to current
                } else {
                    // Reached maximum number of words we can process
                    break;
                }
            }
        } else {
            // Not in a word anymore
            if (inWord) {
                inWord = false;
                *cursor = '\0'; // Terminate the current word
                // Check for consecutive "and" or "or", or if they're at the start or end
                if ((*numWords > 1 && (strcmp(lastWord, "and") == 0 || strcmp(lastWord, "or") == 0) &&
                    (strcmp(words[*numWords - 2], "and") == 0 || strcmp(words[*numWords - 2], "or") == 0)) ||
                    (*numWords == 1 && (strcmp(lastWord, "and") == 0 || strcmp(lastWord, "or") == 0))) {
                    *isValid = false; // Mark query as invalid
                }
            }
        }
    }

    // After processing, check if the last word is "and" or "or"
    if (*numWords > 0 && (strcmp(words[*numWords - 1], "and") == 0 || strcmp(words[*numWords - 1], "or") == 0)) {
        *isValid = false; // Invalid if ends with "and" or "or"
    }
}

// Processes each user query: reads from stdin, validates, tokenizes, scores, and ranks results.
// Continuously prompts for queries until EOF is encountered. Each query is processed to identify
// matching documents, which are then ranked based on relevance and printed to stdout.
static void processQuery(index_t* index, char* pageDir) {
    char query[1000]; // Buffer to store the user's input query.

    // Prompt user for a query.
    printf("Query? ");
    while (fgets(query, sizeof(query), stdin) != NULL) {
        char* words[100]; // Array to store tokenized words.
        int numWords = 0; // Number of words in the query.
        bool isValid = true; // Flag to indicate if the query is syntactically valid.

        // Check if the query only contains letters and spaces.
        if (!isValidQuery(query)) {
            fprintf(stderr, "Error: Query must only contain letters and spaces.\n");
            printf("Query? ");
            continue; // Prompt for another query if invalid.
        }

        // Tokenize the query into individual words and operators.
        tokenize(query, words, &numWords, &isValid);

        // Handle invalid syntax or empty queries.
        if (!isValid || numWords == 0) {
            fprintf(stderr, "Invalid query.\n");
            printf("Query? ");
            continue;
        }

        // Score the query based on the index and rank the results.
        counters_t* result = NULL;
        score(index, numWords, words, &result);

        // Print and rank results if there are any matches.
        if (result != NULL) {
            printf("Query: ");
            for (int i = 0; i < numWords; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");
            rank(result, pageDir); // Rank and print the results.
            counters_delete(result); // Clean up the counters structure.
        } else {
            fprintf(stderr, "No documents match or invalid query.\n");
        }

        printf("-----------------------------------------------\nQuery? "); // Prepare for the next query.
    }
}

// Scores a query by evaluating each word's presence in the index and applying boolean logic.
// Constructs a counter of document IDs (keys) and their scores (values) based on the query.
void score(index_t *index, int numWords, char *words[], counters_t **orSequence) {
    counters_t *andSequence = NULL; // Temporarily stores results of 'AND' operations.
    bool shortCircuit = false; // Used to optimize processing by skipping unnecessary checks.

    if (*orSequence == NULL) {
        *orSequence = counters_new(); // Initialize the OR sequence counter if not already done.
    }

    for (int i = 0; i < numWords; i++) {
        if (strcmp(words[i], "or") == 0) {
            // Merge the current AND sequence into the OR sequence and reset for the next sequence.
            myUnion(orSequence, andSequence);
            andSequence = NULL; // Prepare for the next AND sequence.
            shortCircuit = false; // Reset the short-circuit flag.
        } else if (shortCircuit) {
            // Skip processing until the next OR is found if short-circuiting.
            continue;
        } else if (strcmp(words[i], "and") == 0) {
            // Skip explicit 'AND' since it's implied between sequential words not separated by 'OR'.
            continue;
        } else {
            // Process individual words, scoring them against the index.
            counters_t *wordCounterOG = index_find(index, words[i]);
            if (wordCounterOG == NULL) {
                // If no results for this word, then AND operations with it will always fail.
                shortCircuit = true;
                andSequence = NULL;
            } else {
                counters_t *wordCounter = counters_new();
                counters_copy(wordCounterOG, wordCounter);
                if (andSequence == NULL) {
                    // Start a new AND sequence if this is the first word.
                    andSequence = counters_new();
                    myUnion(&andSequence, wordCounter);
                } else {
                    // Continue the existing AND sequence by intersecting with the current word's results.
                    intersect(&andSequence, wordCounter);
                }
            }
        }
    }

    // Ensure any remaining AND sequence content is merged into the final OR sequence.
    myUnion(orSequence, andSequence);

    // Check if the final OR sequence is empty, indicating no results.
    if (counters_empty(*orSequence) == 0) {
        *orSequence = NULL;
    }
}

// intersect_itemfunc: Applies intersection logic to two counters.
// Used as a helper for counters_iterate in the intersect function.
void intersect_itemfunc(void *arg, const int key, int count) {
    duo_t* args = (duo_t*)arg;
    counters_t* final = args->first;
    counters_t* newCounter = args->second;

    int countInNew = counters_get(newCounter, key); // Get count for the key in newCounter.
    if (countInNew > 0) {
        // Set count in final counter to the minimum of the two counts.
        counters_set(final, key, countInNew < count ? countInNew : count);
    } else {
        // Key doesn't exist in newCounter, so it's not included in the intersection.
        counters_set(final, key, 0);
    }
}

// intersect: Modifies the first counter to include only keys present in both counters.
// Counts are set to the minimum found in either counter, applying 'AND' logic.
void intersect(counters_t **result, counters_t *newCounter) {
    if (*result == NULL || newCounter == NULL) {
        return; // If either counter is NULL, no operation is performed.
    }

    counters_t* final = counters_new(); // Create a new counter for the intersection result.
    duo_t args = {.first = final, .second = newCounter};

    counters_iterate(*result, &args, intersect_itemfunc); // Apply intersection logic.

    counters_delete(*result); // Clean up the original result counter.
    *result = final; // Update result to point to the new intersection result.
}

/*
// MIRROR 
// This function adds count from 'temp' to 'result' for each key.
void union_itemfunc(void *arg, const int key, int count1) {
    duo_t* args = (duo_t*)arg;
    counters_t* final = args->first;
    counters_t* temp = args->second;

    //counters_t *result = (counters_t *)arg;
    int count2 = counters_get(temp, key);

    counters_set(final, key, count1 + count2); // Add counts together
}

// Modifies 'result' to be the union of 'result' and 'temp'.
void myUnion(counters_t **result, counters_t *temp) {
    // Create a new counters set for the union operation
    counters_t *final = counters_new();
    if (final == NULL) {
        fprintf(stderr, "Failed to create new counters for union operation.\n");
        return; // Early return on failure to allocate new counters
    }

    // Add counts from the first counters set, if it exists
    if (*result != NULL) {
        counters_iterate(*result, final, (void (*)(void *, const int, int))counters_set);
    }

    // Add counts from the second counters set, if it exists
    if (temp != NULL) {
        counters_iterate(temp, final, (void (*)(void *, const int, int))counters_set);
    }

    // Clean up the old counters set if it exists
    if (*result != NULL) {
        counters_delete(*result);
    }

    // Point *result to the new combined counters set
    *result = final;
    // If temp is NULL, there's nothing to add to the result.
}
*/


// WORKING VERSION
// This function adds count from 'temp' to 'result' for each key.
void union_itemfunc(void *arg, const int key, int count1) {
    counters_t *result = (counters_t *)arg;
    int count2 = counters_get(result, key);

    counters_set(result, key, count1 + count2); // Add counts together
}

// myUnion: Combines two counters, modifying the first to include all keys from both,
// with their counts summed, applying 'OR' logic to query processing.
void myUnion(counters_t **result, counters_t *temp) {
    if (*result == NULL) {
        *result = temp; // If result is NULL, simply use temp as the result.
    } else if (temp != NULL) {
        counters_iterate(temp, *result, union_itemfunc); // Apply union logic to combine counters.
        counters_delete(temp); // Clean up temp counter after merging.
    }
    // If temp is NULL, no modification is made to result.
}


// Function to copy one counters structure to another
void counters_copy(counters_t *source, counters_t *destination) {
    // Iterate over the source counters, setting each key-count pair in the destination
    counters_iterate(source, destination, (void (*)(void *, const int, int))counters_set);
}

// Checks if a counters structure is empty (i.e., contains no keys with positive counts).
// Iterates over all keys in the counters structure and sums their counts to determine if any positive counts exist.
// Returns 0 if the counters structure has no keys with positive counts, indicating it's "empty" for our purposes.
int counters_empty(counters_t *source) {
    int sum = 0; // Initialize sum of counts to 0.
    counters_iterate(source, &sum, counters_additemfunc); // Sum all counts in the counters.
    return sum; // Return the total sum of counts. A non-zero value indicates the counters is not empty.
}

// Helper function for counters_iterate that adds a specified count to a cumulative total.
// Used in conjunction with counters_iterate to aggregate counts from a counters structure.
// @param arg Pointer to an integer acting as the cumulative total of counts.
// @param key The key for the current counters item (unused in this function).
// @param count The count associated with the current key, to be added to the total.
void counters_additemfunc(void* arg, int key, int count) {
    int *total = (int*)arg; // Cast arg back to pointer to int for the total count.
    *total += count; // Add the count of the current key to the total.
}

// Identifies the document (key) with the maximum score (count) within a counters structure.
// Used with counters_iterate to scan all counters items, keeping track of the max score and corresponding document ID.
// @param arg Pointer to an array of two integers where [0] holds the maximum score found so far, and [1] holds the corresponding document ID.
// @param key The current document ID being examined.
// @param count The score associated with the current document ID.
void findMaxScore(void *arg, const int key, const int count) {
    int *maxData = (int *)arg; // Cast arg to pointer to array of two integers.
    if (count > maxData[0]) { // Check if the current score exceeds the max score found so far.
        maxData[0] = count; // Update the maximum score.
        maxData[1] = key; // Update the document ID associated with the maximum score.
    }
}

// Ranks documents based on their scores and prints the ranked list.
// Iterates through the counters structure to find and print documents in descending order of score.
// Utilizes the findMaxScore function to identify the highest scoring document on each iteration.
// @param result The counters structure containing document IDs as keys and scores as counts.
// @param pageDir The directory containing the page files produced by the Crawler.
void rank(counters_t *result, char* pageDir) {
    while(true) {
        int maxData[2] = {0, -1}; // Array to store the highest score found and corresponding document ID.
        counters_iterate(result, maxData, findMaxScore); // Find the highest scoring document.

        if (maxData[1] == -1) {
            break; // Exit loop if no document has a positive score (i.e., all have been processed).
        }

        // Construct the filename to open the document file.
        char filename[256];
        sprintf(filename, "%s/%d", pageDir, maxData[1]);
        FILE* file = fopen(filename, "r");

        if (file == NULL) {
            fprintf(stderr, "Error opening file %s", filename);
            exit(1); // Exit with error if the file cannot be opened.
        }

        char *url = file_readLine(file); // Read the document's URL from the file.
        fclose(file); // Close the file after reading the URL.

        // Print the document's score, ID, and URL.
        printf("score %d doc %d: %s\n", maxData[0], maxData[1], url);
        free(url); // Free the memory allocated for the URL.

        counters_set(result, maxData[1], 0); // Set the score for this document to 0 to avoid re-selecting it.
    }
}


// Main function: Entry point of the querier program.
// Validates command line arguments, loads the index from the file, and processes queries.
int main(const int argc, char* argv[]) {
    char* pageDirectory = NULL; // Pointer to store the path to the page directory.
    char* indexFilename = NULL; // Pointer to store the path to the index file.

    // Parse and validate command line arguments.
    parseArgs(argc, argv, &pageDirectory, &indexFilename);

    // Load the index from the file.
    index_t* index = index_new();
    if (fileToIndex(index, indexFilename) == NULL) {
        exit(1); // Exit with error if loading the index fails.
    }

    // Process queries from the user.
    processQuery(index, pageDirectory);

    // Cleanup: Free allocated resources.
    index_delete(index); // Delete the index structure.
    free(pageDirectory); // Free the page directory path string.
    free(indexFilename); // Free the index file path string.
    exit(0); // Exit successfully.
}
