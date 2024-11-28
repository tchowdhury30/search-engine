/**
 * querier.h - Header file for the CS50 'Querier' module
 *
 * The Querier is part of the Tiny Search Engine (TSE), designed to process search
 * queries against a search index created by the TSE Indexer, and to retrieve
 * documents that match the queries from a page directory created by the TSE Crawler.
 * This module reads the index file and the page files, evaluates the given search
 * queries submitted via stdin, and returns a ranked list of documents where the
 * search terms were found.
 *
 * Usage:
 * ./querier pageDirectory indexFilename
 * - pageDirectory: the pathname of a directory produced by the Crawler
 * - indexFilename: the pathname of a file produced by the Indexer
 *
 * The querier supports boolean operators 'AND' and 'OR' (in lowercase) to combine
 * search terms. It validates the queries for syntax correctness, tokenizes and
 * normalizes the query terms, evaluates the query against the index, and ranks
 * the results by the frequency of query terms in the documents. The documents are
 * displayed in descending order of their relevance along with their URLs.
 *
 * Input:
 * Queries are read from stdin, one per line. Each query can include one or more
 * search terms, optionally combined using 'AND' and 'OR' operators. The querier
 * is case-insensitive and treats operators and search terms uniformly by
 * converting them to lowercase.
 *
 * Output:
 * For each query, the Querier outputs the ranked list of document IDs, their
 * match score (based on term frequency), and the document's URL. If no documents
 * match the query, a message indicating no matches is printed.
 *
 * Tasnim Chowdhury Feb 2024 
 */

/**
 * Checks if a given query is valid according to the querier's requirements.
 * A valid query contains only letters (case-insensitive) and spaces, and it
 * must not start or end with an operator ('and'/'or') and must not contain
 * consecutive operators.
 *
 * @param query The query string to be validated.
 * @return true if the query is valid, false otherwise.
 */
static bool isValidQuery(char* query);

/**
 * Tokenizes a given query into individual words and operators. Words are
 * normalized to lowercase. This function also validates the token sequence
 * to ensure it adheres to query syntax rules (e.g., no consecutive operators,
 * no operators at start or end).
 *
 * @param query The query string to tokenize.
 * @param words An array of string pointers to store tokens.
 * @param numWords Pointer to an integer where the number of tokens will be stored.
 * @param isValid Pointer to a boolean that will be set to true if the query is
 *                syntactically valid, false otherwise.
 */
void tokenize(char* query, char* words[], int* numWords, bool* isValid);

/**
 * A counters item function for intersection operations. Compares counts of a
 * given key in two counters and updates the count in the destination counter to
 * the minimum of the two counts. If a key does not exist in the second counter,
 * it is removed (count set to 0) from the destination counter.
 *
 * @param arg A pointer to the destination counter (as void* to match counters itemfunc signature).
 * @param key The document ID.
 * @param count1 The count for the document ID in the destination counter.
 */
void intersect_itemfunc(void *arg, const int key, int count1);

/**
 * Intersects two counters, modifying the first to include only keys present in both
 * counters, with their counts set to the minimum found in either counter. This function
 * is used to apply the 'AND' logic in query processing.
 *
 * @param result Pointer to a pointer of the first counter, which will be modified
 *               to store the intersection result.
 * @param newCounter The second counter, to intersect with the first.
 */
void intersect(counters_t **result, counters_t *newCounter);

/**
 * A counters item function for union operations. Adds the count of a given key
 * from one counter to its count in another counter, effectively merging the two.
 *
 * @param arg A pointer to the destination counter (as void* to match counters itemfunc signature).
 * @param key The document ID.
 * @param count1 The count for the document ID to be added to the destination counter.
 */
void union_itemfunc(void *arg, const int key, int count1);

/**
 * Unions two counters, modifying the first to include all keys from both counters,
 * with their counts summed. This function is used to apply the 'OR' logic in query processing.
 *
 * @param result Pointer to a pointer of the first counter, which will be modified
 *               to store the union result.
 * @param temp The second counter, to union with the first.
 */
void myUnion(counters_t **result, counters_t *temp);

/**
 * Processes a query against the given index, applying 'AND' and 'OR' logic as
 * specified by the query tokens. The result is a counter with document IDs as
 * keys and the count of matched words as values.
 *
 * @param index The index structure containing the inverted index data.
 * @param numWords The number of words in the query.
 * @param words Array of words (tokens) from the query.
 * @param result Pointer to a pointer to a counter, which will be allocated and
 *               filled with the query result.
 */
void score(index_t *index, int numWords, char *words[], counters_t** result);

/**
 * A counters item function to find the document with the maximum score. This
 * function is used to rank documents based on the number of times query terms
 * appear in each document.
 *
 * @param arg A pointer to an array of two integers, where the first element will
 *            be updated with the maximum count found, and the second element with
 *            the corresponding document ID.
 * @param key The document ID.
 * @param count The count for the document ID.
 */
void findMaxScore(void* arg, const int key, const int count);

/**
 * Ranks documents based on their scores and prints the ranked list. Documents
 * are printed in descending order of their scores along with their URL.
 *
 * @param result The counter containing document IDs and their scores.
 * @param pageDir The directory containing the page files produced by the Crawler.
 */
void rank(counters_t* result, char* pageDir);

/**
 * Processes each query read from stdin by tokenizing, validating, scoring, and
 * ranking the results. Continues to prompt for queries until EOF is encountered.
 *
 * @param index The index structure containing the inverted index data.
 * @param pageDir The directory containing the page files produced by the Crawler.
 */
static void processQuery(index_t* index, char* pageDir);

/**
 * Copies the content of one counters structure to another. This function iterates through
 * all keys in the source counters structure and sets their corresponding counts in the
 * destination counters structure. If a key exists in the destination that is not in the
 * source, its count remains unchanged.
 * 
 * @param source The counters structure from which to copy counts.
 * @param destination The counters structure where counts will be copied to.
 */
void counters_copy(counters_t *source, counters_t *destination);

/**
 * Determines if a counters structure is empty (i.e., contains no keys with positive counts).
 * This function iterates through the counters structure and checks if there is any key with
 * a count greater than zero.
 * 
 * @param source The counters structure to check for emptiness.
 * @return Returns 1 if the counters structure is empty (no keys with positive counts),
 *         or 0 otherwise.
 */
int counters_empty(counters_t *source);

/**
 * A helper function designed to be used with counters_iterate() that adds a specified
 * count to a key in a counters structure. This function is typically used in the context
 * of aggregating or summing counts across multiple counters structures, such as during
 * a union operation. The function directly modifies the counters structure passed as an
 * argument to counters_iterate().
 * 
 * @param arg A pointer to the counters structure in which the count will be added. This
 *            argument is passed as a void pointer to match the signature expected by
 *            counters_iterate(), and should be cast to (counters_t *) inside the function.
 * @param key The key to which the count will be added.
 * @param count The count to add to the key. If the key does not exist in the counters
 *              structure, it will be added with this count. If the key exists, its count
 *              will be increased by this amount.
 */
void counters_additemfunc(void* arg, int key, int count );
