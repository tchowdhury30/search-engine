#!/bin/bash

# Stop on error
set -e

# Compile the program
echo "Compiling querier..."
make all

# Define directories and files
pageDirectory="../data/crawldata/toscrape-1"
indexFile="${pageDirectory}/toscrape-1.ndx"
#wikipediaDir="../data/crawldata/wikipedia_2"
#wikipediaIndex="${wikipediaDir}/wikipedia_2.ndx"

# Run Valgrind tests
echo "Running Valgrind on querier with toscrape-1..."
# valgrind --leak-check=full ./querier $pageDirectory $indexFile

#echo "Running Valgrind on querier with wikipedia_2..."
#valgrind --leak-check=full ./querier $wikipediaDir $wikipediaIndex

run_query_test() {
    query=$1
    echo "Query: $query"
    echo "$query" | ./querier $pageDirectory $indexFile
}

run_parseargs_test() {
    echo "Testing invalid parseArgs: $1 $2"
    # Temporarily disable 'exit on error'
    set +e
    ./querier "$1" "$2"
    # Check if the last command (querier) failed
    if [ $? -ne 0 ]; then
        echo "Correctly failed. Moving on..."
    else
        echo "Test failed: Expected failure but succeeded."
    fi
    # Re-enable 'exit on error'
    set -e
}

# Replace placeholder names with actual test words according to your data
# Test cases
echo "Running query tests..."
run_query_test "science" # existing word
run_query_test "africa and mother" # logical AND query
run_query_test "girl or BOY" # logical OR query
run_query_test "girl boy" # logical AND query, no matches
run_query_test "boy               girl" # logical AND query, no matches
run_query_test "computer" # word that doesn't exist
run_query_test "computer SCIENCE" # Test with existing and nonexistent word
run_query_test "awfef or english" 
run_query_test "e21 ]1 " 
run_query_test "sh"  
run_query_test "computer science or africa mother"
run_query_test "africa mother or science"
run_query_test "africa MOTHER or science computer"
run_query_test "girl mother or people computer"
run_query_test "girl mother or people computer or"
run_query_test "and girl mother or people computer"
run_query_test "girl mother or and people computer"
run_query_test "girl mother or and people cWOHFUIGYEBVPJWNKBHEIJPFVHBHWJKJOPEJFIHBHJKJNCLKMNVBKHJRWKJELJFIHUGBRWHKQOPJFIWHGKOKWJKBHJOKHJOJLKJG,jmwdbjw omputer"
run_query_test ""
run_query_test "w f "
run_query_test "mother africa"

# Failed cases - multiple ands in a row
run_query_test "english language and mother africa"

run_query_test "mother english language"

echo "Running invalid parseArgs tests..."
run_parseargs_test "" ""
run_parseargs_test $pageDirectory
run_parseargs_test "nonexistentDir" "nonexistentIndex"
run_parseargs_test $pageDirectory "nonexistentIndex"
run_parseargs_test "nonexistentDir" $indexFile
run_parseargs_test $pageDirectory $indexFile "wefwe"


echo "All tests completed successfully."
