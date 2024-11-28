#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <ctype.h>

/*
 * normalize.h - header file for the 'normalize' function.
 *
 * This module provides a simple but essential functionality for text processing
 * in search engines and other text-based applications. The 'normalize' function
 * takes a word (a string of characters) and converts all its characters to lowercase.
 * This operation is fundamental in preparing text data for indexing, searching, and
 * comparison, as it ensures consistency in the representation of words, regardless
 * of how they were originally typed or capitalized.
 *
 * Key functionality:
 * - Converts all characters in a given word to lowercase, modifying the word in place.
 * - Ensures text data can be compared in a case-insensitive manner, enhancing the
 *   flexibility and user-friendliness of text-based applications.
 *
 * The 'normalize' function is especially useful in the context of web crawlers and
 * search engines, where the goal is to index and search text content efficiently
 * and effectively, without being hindered by variations in text capitalization.
 *
 * Usage:
 *   char word[] = "Hello";
 *   normalize(word);
 *   // word is now "hello"
 *
 * Tasnim Chowdhury, CS50, February 2024
 */

/**
 * Normalizes a word by converting all its characters to lowercase.
 * The input word is modified in place.
 *
 * @param word A pointer to a null-terminated string to be normalized.
 */
void normalize(char* word);

#endif // NORMALIZE_H
