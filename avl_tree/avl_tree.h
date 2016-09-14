#ifndef AVL_TREE_H
#define AVL_TREE_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>

/* convenience macros */
/* min max macros, watch out for double evaluation */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct avl_tree avl_tree;

/*
	return a leaf holding a value

	returns pointer to avl_tree if successful
	returns NULL if unsuccessful
*/
avl_tree *avl_new(uint64_t val);

/*
	insert value to tree
	return's subtree holding value
	or NULL if it was unsuccessful

	AVL tree cannot hold duplicate values
*/
avl_tree *avl_insert(avl_tree **tree, uint64_t val);

/*
	search for value from tree
	return's the subtree holding the value
	return's NULL if it's not found
*/
avl_tree *avl_search(avl_tree *tree, uint64_t val);

/*
	deletes the tree
	sets the adress of tree to NULL
*/
void avl_delete(avl_tree **tree);
#endif