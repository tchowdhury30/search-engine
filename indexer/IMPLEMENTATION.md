# CS50 TSE Indexer
## Implementation Spec

This document outlines the implementation specifics for the Tiny Search Engine (TSE) *Indexer*, focusing on data structures, control flow, function prototypes, error handling, and testing plan.

## Data Structures

- **Hashtable**: Maps words to `counters` to track document IDs and occurrences.
- **Counters**: Nested within the hashtable, maps document IDs to frequency counts of words.

## Control Flow

The control flow for the TSE Indexer involves several steps, detailed below with pseudocode for each function involved in the process.

### main
    Parse arguments with parseArgs
    Build index with indexBuild using pageDirectory
    Save the index to indexFilename with indexToFile
    Clean up and free allocated resources


### parseArgs
    Validate argc equals 3
    Validate pageDirectory is a Crawler-produced directory
    Validate indexFilename is writable
    Return pageDirectory, indexFilename

### indexBuild
    Create a new index object
    For each document in dir starting with ID=1
        Load page using pagedir_load
        If page is valid, process with indexPage
    Return the populated index

### indexPage
    Initialize position to 0
    Iterate through words in page:
        Normalize words longer than 2 characters
        Add word to index with index_add
    Free resources appropriately

## Other modules 

## pagedir

### pagedir_validate
    Open dir
    If .crawler file exists
        Return true
    Else
        Return false

### pagedir_load
    Construct filename from dir and docID
    Open file and read content
    Create webpage from content
    Return webpage

## word

### normalize
    Convert word to lowercase

## index

### index_new
    Create hashtable
    Return index

### index_add
    If word exists in index
        Update counter
    Else
        Add word to index with counter


### libcs50

The TSE Indexer leverages several modules from the `libcs50` library to facilitate its operations, ensuring robust data handling and webpage processing capabilities. Key modules used include:

- **`bag`**: A collection that supports adding items and then retrieving them in no particular order. It is not directly used by the Indexer but is integral to the broader TSE infrastructure, particularly for managing tasks in the Crawler.

- **`hashtable`**: Provides a hashtable data structure to efficiently store and retrieve key-value pairs. In the Indexer, it's primarily used to map words to `counters` structures, enabling fast lookups and updates as the index is built.

- **`counters`**: A set of key-count pairs where each key is unique and associated with a count. This module is essential for tracking the frequency of each word across different documents, as it allows the Indexer to maintain a count of word occurrences within specific document IDs.

- **`webpage`**: This module represents webpages as objects, enabling the fetching of webpage content from the Internet and the scanning of a webpage for URLs. While the Indexer does not fetch new pages, it utilizes the `webpage` module to load and process saved webpage files from the Crawler's output.

The integration of these modules from `libcs50` into the Indexer ensures efficient data management and supports the Indexer's goal of creating a searchable structure from crawled web pages. By leveraging existing `libcs50` functionalities, the Indexer can focus on its core task of building an inverted index without needing to reimplement basic data structures and webpage handling mechanisms.

## Function Prototypes

### indexer 

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `indexer.h` and is not repeated here.

```c
int main(const int argc, char* argv[]);
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
index_t* indexBuild(const char* dir);
bool indexPage(index_t* index, webpage_t* page, int docID);

```
### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* dir);
webpage_t* pagedir_load(const char* dir, int docID); 
```

## Error Handling and Recovery
The TSE Indexer implements robust error handling mechanisms to ensure stability and reliability:

**Command-Line Arguments**: Checks for the correct number of arguments and validates their formats. If an error is detected, the program exits with a specific error message, guiding the user to correct usage.

**Directory and File Validation**: Verifies that the provided page directory exists, contains the expected .crawler file, and is readable. Similarly, it ensures that the index file can be created or overwritten. Any access or permission issues lead to an immediate, informative error message and program termination.

**Memory Allocation**: All dynamic memory allocations are checked for success. On failure (e.g., due to insufficient memory), the program releases any allocated resources and terminates gracefully, preventing memory leaks.

**File Operations**: Reads from and writes to files are monitored for errors. Should any file operation fail (e.g., due to I/O errors), the program cleans up resources, potentially logs the error, and exits cleanly.

**Data Integrity**: Before processing any data (e.g., words from documents), the Indexer validates the integrity of the data. This includes checking word lengths, ensuring counters are correctly updated, and more.

## Testing Plan

### Unit Testing: 
**Individual Module Testing**: Each module (`index`, ``word``, `pagedir`, etc.) is tested in isolation to verify its functionality. Mock objects and data may be used to simulate interactions.

**Function-Level Tests:** Critical functions within each module (e.g., `index_add`, `normalize`) are tested with a range of input values, including boundary cases and potential error conditions.

### Integration Testing:  
**End-to-End Process**: The complete workflow of the Indexer, from reading the page directory to writing the index file, is tested to ensure that all components work together as expected.

**Index File Verification:** The output index files are validated against known correct indexes to verify accuracy.

**Stress Testing**: The Indexer is run against large datasets to test its performance and stability under load.

### Validation Testing: 
**indextest Program:** Uses the indextest program to read an index file and then write it out to a new file. The contents of both files are compared to ensure they match exactly.

**Memory Leak Detection**: Tools like Valgrind are used to detect any memory leaks or errors, ensuring the Indexer manages memory correctly throughout its operation.

**Regression Testing**: Automated scripts run a suite of tests for each new version of the Indexer to quickly identify any regressions or new errors introduced during development.
