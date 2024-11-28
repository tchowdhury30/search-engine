/*
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Tasnim Chowdhury, 1/23/24
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "mem.h"
 

/**************** local types ****************/
typedef struct cnode {
  int key;              // integer key
  int tally;		      // counter 
  struct cnode *next;	  // link to next node
} cnode_t;

/**************** global types ****************/
typedef struct counters {
  struct cnode *head;	      // head of the list of items in counters
} counters_t;

/**************** functions ****************/

/* Create a new (empty) counter structure; return NULL if error. */
counters_t *counters_new(void) 
{
    counters_t *counters = mem_malloc(sizeof(counters_t));
    if (counters == NULL) {
        return NULL; //error allocating counters
    } else {
        // initialize contents of counters structure
        counters->head = NULL;
        return counters;
    }
}

/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Return the new value of the counter related to the indicated key.
 *  and 0 on error (if ctrs is NULL or key is negative)
 */
int counters_add(counters_t *ctrs, const int key)
{
    if (ctrs != NULL && key >= 0) {
        // Check if the key already exists in the counters
        int exist = 0;
        for (cnode_t *node = ctrs->head; node != NULL; node = node->next) {
            if (node->key==key) {
                node->tally++;
                exist = 1;
            }
        }
        if (!exist) {
            // Allocate memory for the new node
            cnode_t *new_node = mem_malloc(sizeof(cnode_t));
            //node insertion - create counter for key and initialize to 1 
            new_node->key = key;
            new_node->tally = 1;
            new_node->next = ctrs->head;
            ctrs->head = new_node;
            return new_node->tally;
        }
        
  }
  return 0; //failed 
  
}

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key) 
{
    if (ctrs != NULL) {
        for (cnode_t *node = ctrs->head; node != NULL; node = node->next) {
            if (node->key==key) {
                return node->tally; //key found
            }
        }
    }
    return 0; // key not found or NULL
}

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. 
 * false if ctrs is NULL, if key < 0 or count < 0, or if out of memory.
 *   otherwise returns true.
 */
bool counters_set(counters_t *ctrs, const int key, int count)
{
    if (ctrs != NULL && key > 0 && count > 0) {
        int exist = 0;
        for (cnode_t *node = ctrs->head; node != NULL; node = node->next) {
            if (node->key==key) {
                node->tally = count; //key found
                exist = 1;
                return true;
            }
        }
        if (!exist) { //create new node, key does not exist 
            // Allocate memory for the new node
            cnode_t *new_node = malloc(sizeof(cnode_t));
            if (new_node == NULL) {
                return false; // Error allocating memory
            }
            //node insertion - create counter for key and initialize to 1 
            new_node->key = key;
            new_node->tally = count;
            new_node->next = ctrs->head;
            ctrs->head = new_node;
            return true;

        }
    }
    return false;
}

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp)
{
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);
            for (cnode_t *node = ctrs->head; node != NULL; node = node->next) {
                // print this node
                fprintf(fp, "%d",node->key);
                fputc(':',fp);
                fprintf(fp,"%d",node->tally);
                fputc(',', fp);
            }
            fprintf(fp,"}\n"); //completed counters 
        } else { //null counters 
            fputs("(null)", fp);
        }
  } //ignored 
}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, int count))
{
    if (ctrs != NULL && itemfunc != NULL) {
        // call itemfunc with arg, on each item
        for (cnode_t *node = ctrs->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->tally);
        }
    }
}

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs)
{
    if (ctrs != NULL) {
        for (cnode_t *node = ctrs->head; node != NULL; ) {
            cnode_t *next = node->next;	    // remember what comes next
            mem_free(node);			    // free the node
            node = next;			    // and move on to next
        }
        mem_free(ctrs);
    }
}
 