/*
 * crawler.c - CS50 'crawler' module
 *
 * see crawler.h for more information.
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * seedURL is an ‘internal’ directory, to be used as the initial URL
 * pageDirectory is the (existing) directory in which to write downloaded webpages
 * maxDepth is an integer in range [0..10] indicating the maximum crawl depth.
 *
 * Tasnim Chowdhury, 1/31/24
 */


#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"

const int tableSize = 200;

// Prototypes for functions used
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void logr(const char *word, const int depth, const char *url);


/*
 * Function: main
 * ---------------
 * The entry point for the crawler program.
 * It parses command-line arguments, initializes the crawler with the seed URL,
 * page directory, and maximum depth, then starts the crawling process.
 *
 * Parameters:
 *  - argc: Number of command-line arguments
 *  - argv: Array of command-line argument strings
 *
 * Returns:
 *  - Exit status of the program (0 for success, non-zero for failure)
 *
 * Notes:
 *  - The function validates command-line arguments using `parseArgs`.
 *  - On success, it invokes `crawl` to start the crawling process.
 *  - It handles incorrect number of arguments by displaying usage information.
 */
int main(const int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }
    char* normalizedSeedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth;

    parseArgs(argc, argv, &normalizedSeedURL, &pageDirectory, &maxDepth);
    crawl(normalizedSeedURL, pageDirectory, maxDepth);
    exit(0);
}

// log one word (1-9 chars) about a given url                                   
static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}

/*
 * Function: parseArgs
 * --------------------
 * Parses and validates command-line arguments for the crawler.
 *
 * This function ensures that the command-line input is correct by normalizing 
 * and validating the seed URL, initializing the page directory, and checking 
 * the maxDepth range. If any input is invalid, it exits the program with an 
 * error message.
 *
 * Parameters:
 *  - argc: Number of command-line arguments
 *  - argv: Array of command-line argument strings
 *  - seedURL: Pointer to store the normalized seed URL
 *  - pageDirectory: Pointer to store the page directory path
 *  - maxDepth: Pointer to store the maximum crawl depth
 *
 * Exits:
 *  - Program exits if any argument is invalid with an error message.
 */
static void parseArgs(const int argc, char* argv[], 
                      char** seedURL, char** pageDirectory, int* maxDepth) {

    /* Normalize the seed URL and check if it's an internal URL */
    char* normalizedSeedURL = normalizeURL(argv[1]);
    if (normalizedSeedURL == NULL || !isInternalURL(normalizedSeedURL)) {
        fprintf(stderr, "Invalid or non-normalized Seed URL.\n");
        free(normalizedSeedURL); // Clean up allocated memory
        exit(1); // Exit on invalid seed URL
    }

    /* Initialize the page directory */
    if (!pagedir_init(argv[2])) {
        fprintf(stderr, "Failed to initialize the page directory: %s\n", argv[2]);
        free(normalizedSeedURL); // Clean up before exiting
        exit(1); // Exit on page directory initialization failure
    }

    /* Assign the parsed arguments to the respective output parameters */
    *seedURL = normalizedSeedURL;
    *pageDirectory = argv[2];

    /* Validate and assign maxDepth */
    int depth = atoi(argv[3]);
    if (depth < 0 || depth > 10) {
        fprintf(stderr, "maxDepth out of range.\n");
        free(normalizedSeedURL);
        exit(1); // Exit on invalid maxDepth
    }
    *maxDepth = depth;
    return; // Successful argument parsing
}


/*
 * Function: pageScan
 * -------------------
 * Scans a given webpage for URLs and processes each found URL. 
 * It extracts URLs, filters out non-internal ones, checks for duplicates using a hashtable, 
 * and adds new URLs to both the hashtable and a bag for further crawling.
 *
 * Parameters:
 *  - page: The webpage to be scanned for URLs
 *  - pagesToCrawl: The bag where new webpages to be crawled are added
 *  - pagesSeen: The hashtable used to track URLs that have already been seen
 *
 * Notes:
 *  - The function ignores URLs that are either non-internal or already seen.
 *  - New URLs are duplicated, added to the hashtable, and wrapped in a new webpage object
 *    before being added to the bag.
 *  - The function handles memory allocation failures and avoids duplicate entries.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    int pos = 0;
    char *result;

    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
        if (isInternalURL(result)) {
            if (hashtable_find(pagesSeen, result) == NULL) {
                char* urlForWebpage = strdup(result);
                if (urlForWebpage == NULL) {
                    fprintf(stderr, "Failed to duplicate URL.\n");
                    free(result);
                    continue;
                }
                if (!hashtable_insert(pagesSeen, urlForWebpage, urlForWebpage)) {
                    free(urlForWebpage); // Clean up if insertion fails
                } else {
                    webpage_t *newPage = webpage_new(urlForWebpage, webpage_getDepth(page) + 1, NULL);
                    if (newPage == NULL) {
                        continue;
                    }
                    bag_insert(pagesToCrawl, newPage);
                    logr("Added", webpage_getDepth(page) + 1, urlForWebpage); 
                }
            } else {
                logr("IgnDupl", webpage_getDepth(page), result);
            }
            free(result); // Free the result
        } else {
            logr("IgnExtrn", webpage_getDepth(page), result);
            free(result);
        }
    }
}

/*
 * Function: crawl
 * ----------------
 * Crawls webpages starting from a seed URL up to a specified maximum depth,
 * and saves each webpage to the given page directory.
 *
 * This function initializes necessary data structures (a hashtable and a bag)
 * to keep track of visited URLs and URLs to visit. It performs the crawling 
 * process by fetching webpages, scanning for new URLs, and saving the content.
 * In case of failure in fetching a webpage, it logs an error message.
 *
 * Parameters:
 *  - seedURL: The initial URL from which to start crawling
 *  - pageDirectory: The directory where the webpages are saved
 *  - maxDepth: The maximum depth for crawling
 *
 * Notes:
 *  - The function uses hashtable and bag data structures from libcs50.
 *  - Each webpage is assigned a unique document ID as it's saved.
 *  - Crawling is paused for 1 second between fetching webpages to avoid overloading servers.
 *  - The function handles errors in data structure creation and webpage fetching.
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) 
{
    /* Initialize hashtable and bag */
    hashtable_t* ht = hashtable_new(tableSize);
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hashtable.\n");
        return;
    }

    bag_t* bag = bag_new();
    if (bag == NULL) {
        fprintf(stderr, "Failed to create bag.\n");
        hashtable_delete(ht, webpage_delete);
        return;
    }

    /* Create and add seed webpage to bag */
    webpage_t *seed = webpage_new(seedURL, 0, NULL);
    if (seed == NULL) {
        fprintf(stderr, "Failed to create seed webpage.\n");
        hashtable_delete(ht, webpage_delete);
        bag_delete(bag, NULL);
        return;
    }
    bag_insert(bag, seed);
    int docID = 1; // Start document ID from 1

    /* Crawl process */
    webpage_t *curr;
    while ((curr = bag_extract(bag)) != NULL) {
        sleep(1); // Pause to avoid server overload
        if (webpage_fetch(curr)) {
            logr("Fetched", webpage_getDepth(curr), webpage_getURL(curr));
            pagedir_save(curr, pageDirectory, docID++);
            if (webpage_getDepth(curr) < maxDepth) {
                logr("Scanning", webpage_getDepth(curr), webpage_getURL(curr));
                pageScan(curr, bag, ht);   
            }
        } else {
            fprintf(stderr, "Failed to fetch webpage: %s\n", webpage_getURL(curr));
        }
        webpage_delete(curr); // Clean up after processing
    }

    /* Clean up data structures */
    hashtable_delete(ht, NULL);
    bag_delete(bag, webpage_delete);
}

