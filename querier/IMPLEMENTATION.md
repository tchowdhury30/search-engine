# CS50 TSE Querier Implementation Specification

## Overview

This specification details the implementation of the TSE Querier, focusing on the execution of search queries against a previously generated index by the TSE Indexer, using data collected by the TSE Crawler. The Querier operates through command-line interaction, supporting complex queries with logical operators "and" and "or".

## Data Structures 

1. **Index**: Utilizes a hashtable to store the index loaded from a file. Each entry in the hashtable corresponds to a unique word found in the documents, mapping to a counters structure that tracks the occurrence of that word in various document IDs.

2. **Query**: An array of strings (tokens) representing the parsed user query. This array alternates between words and operators, facilitating the evaluation process.

3. **Document Scores**: A counters structure that keeps track of the cumulative score for each document ID as queries are evaluated. The score represents the relevance of a document to the quer

## Control Flow and Pseudo Code

### Main Function
1. Argument Parsing and Validation: parseArgs checks the correctness of command-line arguments, ensuring the presence of a valid page directory and index filename.

2. Index Loading: loadIndex reads the index from the specified file into an in-memory hashtable structure.

3. Query Processing Loop:
    - Prompt the user for a query.
    Read the query from stdin.
    If the query is empty or invalid, display an error and prompt again.
    Otherwise, process the query against the loaded index.

### Query Processing
1. Tokenization: Break down the query into individual words and operators. Normalize words to lowercase.
2. Validation: Ensure the query follows logical syntax rules (e.g., no starting or ending with operators, no consecutive operators
3. Evaluation: For each token in the query:
    - If it's a word, retrieve its document counters from the index
    - Apply logical operators to combine counters:
        - "and" results in the intersection of document sets.
        - "or" results in the union of document sets.
4. Scoring and Ranking: Calculate scores based on the frequency of query terms in documents. Sort documents by their scores in descending order.
5. Output: For each document in the sorted list, print the document ID, score, and URL to stdout.

## Functional Decomposition into Modules

### Parse Query and Tokenization
    Function parseAndTokenizeQuery(queryString)
        Initialize an empty list for tokens
        Convert queryString to lowercase for uniform processing
        For each character in queryString
            If character is alphabetic or numeric
                Append character to currentWord
            Else if character is space
                If currentWord is not empty
                    Add currentWord to tokens list
                    Clear currentWord
        If currentWord is not empty at the end
            Add currentWord to tokens list
        Validate the sequence of tokens
        Return tokens


The initial step involves breaking down the user's input into manageable tokens (words and operators) and validating the structure of the query.

### Validate Query Syntax
    If first or last token is an operator
        Return false with an error message
    For each token in tokens
        If current token and next token are both operators
            Return false with an error message
    Return true

After tokenization, ensure the query follows logical rules (no consecutive operators, no leading or trailing operators).

### Evaluate Query
    Function evaluateQuery(tokens, index)
    Initialize an empty stack for counters (to hold intermediate results)
    For each token in tokens
        If token is a word
            Retrieve the document counters for this word from the index
            Push the counters onto the stack
        Else if token is an operator (AND or OR)
            Pop the top two counters off the stack
            Apply the corresponding operation (intersect or union)
            Push the result back onto the stack
    Final result will be the top of the stack
    Return the final result counters

The core logic where the parsed and validated query is evaluated against the loaded index. This involves applying AND/OR logic to combine results from different tokens.

### Apply AND Logic (Intersection)
    Function intersectCounters(counter1, counter2)
    Initialize a new counters for the intersection result
    For each document ID in counter1
        If document ID also exists in counter2
            Set its count in result counters to the minimum of its counts in counter1 and counter2
    Return result counters

When encountering the "AND" operator, intersect the document sets from two counters, resulting in documents that contain all words.

### Apply OR Logic (Union)
    Function unionCounters(counter1, counter2)
    Initialize a new counters for the union result
    Copy all document IDs and counts from counter1 to result counters
    For each document ID in counter2
        Add its count to the corresponding count in result counters (if already present, else set)
    Return result counters

For the "OR" operator, combine document sets, adding up counts when documents appear in both sets.

### Scoring and Ranking Results
    Function rankAndPrintResults(results, pageDirectory)
    Convert results counters into a list of (document ID, score) pairs
    Sort the list in descending order by score
    For each pair in the sorted list
        Retrieve the document's URL from the page directory using document ID
        Print document ID, score, and URL

Finally, after evaluating the entire query, rank the documents based on their scores (counts).

## Function Prototypes
### querier
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in querier.h and is not repeated here.

```c static bool isValidQuery(char* query);
void tokenize(char* query, char* words[], int* numWords, bool* isValid);
void intersect_itemfunc(void *arg, const int key, int count1);
void intersect(counters_t **result, counters_t *newCounter);
void union_itemfunc(void *arg, const int key, int count1);
void myUnion(counters_t **result, counters_t *temp);
void score(index_t *index, int numWords, char *words[], counters_t** result);
void findMaxScore(void* arg, const int key, const int count);
void rank(counters_t* result, char* pageDir);
static void processQuery(index_t* index, char* pageDir);
```

## Error Handling Strategies

- Invalid command-line arguments result in an error message and termination of the program.
- Failure to load the index from the specified file results in an error message and termination.
- Queries that do not conform to the expected syntax are rejected with an error message, prompting the user for another query.
- Memory allocation failures are checked and handled gracefully, with appropriate cleanup and error messages.

## Testing and Validation

- **Syntax Validation Tests**: Verify that the Query Processor correctly handles a range of valid and invalid queries.
- **Evaluation Logic Tests**: Test the Search Evaluator with various queries to ensure accurate document matching and correct application of "and" and "or" logic.
- **Ranking Tests**: Confirm that the Result Ranker correctly scores and orders documents according to the frequency of query terms.
End-to-End Tests: Execute comprehensive tests using real index files to validate the entire process from query input to result output.
- **Memory Leak Detection**: Utilize tools like Valgrind to identify and resolve any memory leaks to ensure efficient resource management.