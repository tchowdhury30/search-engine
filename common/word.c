/*
 * word.c - CS50 'word' module
 *
 * word utilities - normalize
 *
 * Tasnim Chowdhury, 2/4/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//given a word, return a normalized version of it 
void normalize(char* word) 
{
    if (word == NULL) {
        return; // Early return if the input word is NULL to avoid undefined behavior
    }

    // Loop through each character of the word until the null terminator is reached
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]); // Convert each character to lowercase
    }
}
