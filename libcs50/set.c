/*
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Tasnim Chowdhury, 1/22/24
 */

#ifndef __SET_H
#define __SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "mem.h"


/**************** local types ****************/
typedef struct setnode {
  char* key;              // pointer to key 
  void *item;		      // pointer to data for this item
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;

/**************** functions ****************/

/* Create a new (empty) set; return NULL if error. */
set_t *set_new(void) 
{
    //set_t *set = mem_malloc_assert(sizeof(setnode_t), "Error allocating set.");
    set_t *set = malloc(sizeof(set_t));

    // initialize contents of set structure
    set->head = NULL;
    return set;

}

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true if new item was inserted.
 */
bool set_insert(set_t *set, const char *key, void *item)
{
    if (set != NULL && item != NULL) {
        // Check if the key already exists in the set
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            if (strcmp(node->key, key) == 0) {
                return false; // Key already exists
            }
        }
        // Allocate memory for the new node
        setnode_t *new_node = malloc(sizeof(setnode_t));
        if (new_node == NULL) {
            return false; // Error allocating memory
        }
        // Allocate memory for the key and copy it
        new_node->key = malloc(strlen(key) + 1);
        if (new_node->key == NULL) {
            free(new_node); // Clean up allocated memory
            return false; // Error allocating memory
        }
        strcpy(new_node->key, key);
        new_node->item = item;
        new_node->next = set->head;
        set->head = new_node;
        return true; // New item was inserted
  }
  return false; // param NULL
}

/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, const char *key) 
{
    if (set != NULL && key != NULL) {
        // Check if the key exists in the set
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            if (strcmp(node->key, key) == 0) {
                return node->item; //Found key
            }
        }
    }
    return NULL; //args NULL or key not found
}

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
    if (fp != NULL) {
        if (set != NULL) {
            fputc('{', fp);
            for (setnode_t *node = set->head; node != NULL; node = node->next) {
                // print this node
                if (itemprint != NULL) {  // print the node's item
                    (*itemprint)(fp, node->key, node->item);
                    fputc(',', fp);
                }
            }
            fputc('}', fp); //completed set 
        } else { //null set 
            fputs("(null)", fp);
        }
  } //ignored 
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
    if (set != NULL && itemfunc != NULL) {
        // call itemfunc with arg, on each item
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->item);
        }
  } 
}

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) ) 
{
    if (set != NULL) {
        for (setnode_t *node = set->head; node != NULL; ) {
            if (itemdelete != NULL) {		    // if possible...
                (*itemdelete)(node->item);	    // delete node's item
                free(node->key);          // free the key
            }
            setnode_t *next = node->next;	    // remember what comes next
            free(node);			    // free the node
            node = next;			    // and move on to next
        }
        free(set);
  }
}

#endif // __SET_H