/*
 * avl_tree.c
 *
 * Copyright (C) 2016 Tim Miller
 */

#include <stdlib.h> /* needed for NULL */
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

#include "avl_tree.h"

struct avl_tree {
	uint64_t val;
	avl_tree *left, *right;
	avl_tree *parent;
	avl_tree **self;
	int height;
};

/* create a new empty avl_tree */
avl_tree *avl_new(uint64_t val){
	avl_tree *new = malloc(sizeof(avl_tree));
	new->val = val;
	new->height = 0;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	return new;
}

/* update height, starting from the top working down,
 * as opposed to starting from the bottom working up */
void avl_update_height_root(avl_tree *tree){
	if(tree->left){
		avl_update_height_root(tree->left);
	}
	if(tree->right){
		avl_update_height_root(tree->right);
	}
	
	if(tree->right && tree->left){
		tree->height = (
			tree->left->height > tree->right->height ? 
			tree->left->height : tree->right->height) + 1;
	}
	else if (tree->right){
		tree->height = tree->right->height + 1;
	}
	else if(tree->left){
		tree->height = tree->left->height + 1;
	}
	else{
		tree->height = 0;
	}
}

/* starting at root, update each childs parent */
void update_parent(avl_tree *tree){
	if(tree->left){
		tree->left->parent = tree;
		update_parent(tree->left);
	}
	if(tree->right){
		tree->right->parent = tree;
		update_parent(tree->right);
	}
}

/* rotate left-right */
void avl_rotate_rl(avl_tree **tree){
	avl_tree *a = *tree, *b = (*tree)->right, *c = (*tree)->right->left;
	a->right = c->left;
	b->left = c->right;
	c->left = a;
	c->right = b;
	*tree = c;
	/* update parents */
	(*tree)->parent = NULL;
	update_parent(*tree);
	/* update heights */
	(*tree)->height = 1;
	(*tree)->left->height = 0;
	(*tree)->right->height = 0;
}

/* get the root of the whole tree */
avl_tree *avl_root(avl_tree *tree){
	if(tree->parent)
		return avl_root(tree->parent);
	else
		return tree;
}

/* rotate right */
void avl_rotate_r(avl_tree **tree){
	avl_tree *a = *tree, *b = (*tree)->left;
	a->left = b->right;
	b->right = a;
	*tree = b;

	(*tree)->parent = NULL;
	update_parent(*tree);
	avl_update_height_root(avl_root(*tree));
}

/* rotate left */
void avl_rotate_l(avl_tree **tree){
	avl_tree *a = *tree, *b = (*tree)->right;
	a->right = b->left;
	b->left = a;
	*tree = b;
	/* update parents */
	(*tree)->parent = NULL;
	update_parent(*tree);
	/* update heights */
	avl_update_height_root(avl_root(*tree));
}

/* rotate left-right */
void avl_rotate_lr(avl_tree **tree){
	avl_tree *a = *tree, *b = (*tree)->left, *c = (*tree)->left->right;
	b->right = c->left;
	a->left = c->right;
	c->left = b;
	c->right = a;
	*tree = c;
	/* update parents */
	(*tree)->parent = NULL;
	update_parent(*tree);
	/* update heights */
	(*tree)->height = 1;
	(*tree)->left->height = 0;
	(*tree)->right->height = 0;
}

/* balance the tree */
void avl_balance(avl_tree **tree){
	/* first attempt to balance children */
	if((*tree)->left){
		avl_balance(&((*tree)->left));
	}
	if((*tree)->right){
		avl_balance(&((*tree)->right));
	}

	/* a tree without grandkids can't be unbalanced */
	if((*tree)->height < 2){
		return;
	}

	/* figure out if it's unbalanced */
	int balfactor;
	if((*tree)->left && (*tree)->right){
		balfactor = (*tree)->left->height - (*tree)->right->height;
	}
	else if((*tree)->left){
		balfactor = (*tree)->left->height - -1;
	}
	else if((*tree)->right){
		balfactor = -1 - (*tree)->right->height;
	}
	if(abs(balfactor) > 1){
		/* unbalanced */
		if(balfactor < 0){
			/* right unbalanced */
			if((*tree)->right->left){
				/* right-left rotate */
				avl_rotate_rl(tree);
			}
			if((*tree)->right->right){
				/* left rotate */
				avl_rotate_l(tree);
			}
		}
		if(balfactor > 0){
			/* left unbalanced */
			if((*tree)->left->right){
				/* left-right roate */
				avl_rotate_lr(tree);
			}
			if((*tree)->left->left){
				/* right rotate */
				avl_rotate_r(tree);
			}
		}
	}
}

/* update the height, starting at a child, working its way up
 * O(n) where n is the number of ancestors to the tree
 */
void avl_update_height(avl_tree *tree){
	/* NULL check */
	if(!tree)
		return;
	/* if it's a leaf, it's height is 0 */
	if(!(tree->left) && !(tree->right)){
		tree->height = 0;
	}
	/* if it has both children, then height is max(l.h, r.h) +1 */
	else if(tree->left && tree->right){
		tree->height = (
			tree->left->height > tree->right->height ? 
			tree->left->height : tree->right->height) + 1;
	}
	/* tree only has left */
	else if(tree->left){
		tree->height = tree->left->height + 1;
	}
	/* tree only has right */
	else if(tree->right){
		tree->height = tree->right->height + 1;
	}
	/* update parent */
	avl_update_height(tree->parent);
}

avl_tree *avl_insert(avl_tree **tree, uint64_t val) {
	avl_tree *result;
	bool need_update = false;
	/* if tree is NULL, then there is no tree, create one */
	if(!(*tree)){
		(*tree) = avl_new(val);
		return *tree;
	}
	/* if tree's val matches val, throw out, return NULL */
	if((*tree)->val == val){
		return NULL;
	}
	/* if val is < tree's val, go left */
	if(val < (*tree)->val){
		/* if left exists, recurse left */
		if((*tree)->left){
			result = avl_insert(&((*tree)->left), val);
		}
		/* if no left, val becomes new left node */
		else{
			(*tree)->left = avl_new(val);
			/* assign parent */
			(*tree)->left->parent = *tree;
			result = (*tree)->left;
			need_update = true;
		}
	}
	/* if val is > tree's val, go left */
	if(val > (*tree)->val){
		/* if right exists, recurse right */
		if((*tree)->right){
			result = avl_insert(&((*tree)->right), val);
		}
		/* if no right, val becomes new right node */
		else{
			(*tree)->right = avl_new(val);
			/* assign parent */
			(*tree)->right->parent = *tree;
			result = (*tree)->right;
			need_update = true;
		}
	}
	/* update height and balance here */
	if(need_update){
		avl_update_height(result);
	}
	/* if root do the balance thing */
	if((*tree)->parent == NULL){
		avl_balance(tree);
	}
	return result;
}

/*
 * search the tree for a value
 * return NULL if not found
 * return a pointer to the subtree holding
 * the value if it's found
 */
avl_tree *avl_search(avl_tree *tree, uint64_t val) {
	/* NULL check */
	if(!tree)
		return NULL;
	/* found it! */
	if(tree->val == val)
		return tree;
	/* note that we don't check that the left or right node exists before calling avl_search
	 * this is because we do a NULL check at the beginning of the function,
	 * so that step can be skipped :)
	 * this could be seen as an unnecessary addition to the callstack but it sure looks neato
	 */
	if(val < tree->val)
		return avl_search(tree->left, val);
	if(val > tree->val)
		return avl_search(tree->right, val);
	/* control will never reach here */
	assert(false);
	return NULL;
}

/* 
 * Deletes children first, then free's the struct
 */
void avl_delete(avl_tree **tree) {
	/* check NULL */
	if(!(*tree))
		return;
	/* delete children */
	avl_delete(&((*tree)->left));
	avl_delete(&((*tree)->right));
	free(*tree);
	*tree = NULL;
}

/* print tree 
 * lovingly cobbled together from stackoverflow
 * but you're not grading this function, so that's okay
 * it's really just here because I'm tired of openning ddd to check simple stuff
 */
void avl_print(avl_tree *tree, int depth){
	if(!tree)
		return;
	avl_print(tree->left, depth + 1);
	if(depth != 0){
		int i;
		for(i = 0; i < (depth - 1); i++){
			printf("|\t");
		}
		printf("|-------%"PRIu64"(%d)\n", tree->val, tree->height);
	}
	else{
		printf("%"PRIu64"(%d)\n", tree->val, tree->height);
	}
	avl_print(tree->right, depth+1);
}