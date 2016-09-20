/*
 * avl_tree.h
 *
 * Copyright (C) 2016 William "Amos" Confer
 */

#ifndef avl_tree_h
#define avl_tree_h


#include <stdint.h>


typedef struct avl_tree avl_tree;


/*
 * Inserts a value into a tree and ensures the resulting tree is height
 * balanced.
 * 
 * A NULL tree indicates an empty tree.
 *
 * Returns a pointer to the root of the subtree, post insert, with the
 * value as its root or NULL if the value was already in the tree.
 */
extern avl_tree *avl_insert(avl_tree **t, uint64_t val);


/*
 * Searches the given tree for a subtree rooted by a value.
 *
 * A NULL tree indicates an empty tree.
 *
 * Returns a pointer to the root of the subtree rooted by the value.
 */
extern avl_tree *avl_search(avl_tree *t, uint64_t val);


/*
 * Deletes the subtree given with deallocation and NULLs the pointer.
 */
extern void avl_delete(avl_tree **t);


#endif /* avl_tree_h */
