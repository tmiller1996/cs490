#include "avl_tree.h"

struct avl_tree{
	uint64_t val;
	avl_tree *left, *right, *parent;
	int height;
};

avl_tree *avl_new(uint64_t val){
	avl_tree *new = (avl_tree*)malloc(sizeof(avl_tree));
	if(new == NULL)
		return NULL;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	new->val = val;
	new->height = 0;
	return new;
}

/*
	recurse through tree, updating height
	height = MAX(tree.left.height, tree.right.height) + 1
*/
void avl_update_height(avl_tree *tree){
	if(tree->left && !(tree->right)){
		tree->height = tree->left->height + 1;
	}
	else if (!(tree->left) && tree->right){
		tree->height = tree->right->height + 1;
	}
	else if(tree->right && tree->left){
		tree->height = MAX(tree->left->height, tree->right->height) + 1;
	}
	else{
		/* leaf */
		assert(tree->height == 0);
	}
	if(tree->parent){
		avl_update_height(tree->parent);
	}
}

void avl_rotate_right(avl_tree **tree){

}

void avl_rotate_left(avl_tree **tree){
	
}

/* balance the tree, if balanced recurse upward */
void avl_balance(avl_tree **tree){
	/* balfactor = height(left) - height(right) */
	int balfactor;
	if((*tree)->left && (*tree)->right){
		balfactor = abs((*tree)->left->height - (*tree)->right->height);
	}
	else if((*tree)->left){
		balfactor = (*tree)->left->height;
	}
	else if((*tree)->right){
		balfactor = (*tree)->right->height;
	}
	else{
		balfactor = 0;
	}

	if(balfactor >= 2){
		if((*tree)->left){
			if((*tree)->left->left){
				/* rotate right */
				avl_rotate_right(tree);
			}
			else if((*tree)->left->right){
				/*
				move tree.left.right to tree.left.left
				tree.left.right = NULL
				rotate right
				*/
				(*tree)->left->left = (*tree)->left->right;
				(*tree)->left->right = NULL;
				avl_rotate_right(tree);
			}
		}
		if((*tree)->right){
			if((*tree)->right->right){
				/* rotate left */
				avl_rotate_left(tree);
			}
			if((*tree)->right->left){
				/*
				move tree.right.left to tree.right.right
				tree.right.left = NULL
				rotate left
				*/
				(*tree)->right->right = (*tree)->right->left;
				(*tree)->right->left = NULL;
				avl_rotate_left(tree);
			}
		}
	}
	/* update each child */
	if((*tree)->left){
		avl_balance(&((*tree)->left));
	}
	if((*tree)->right){
		avl_balance(&((*tree)->right));
	}
	/* leafs call update_height */
	if(!((*tree)->left) && !((*tree)->right)){
		avl_update_height(*tree);
	}
	return;
}

avl_tree *avl_insert(avl_tree **tree, uint64_t val){
	avl_tree *result = NULL;
	/*
		go left if val is less
		go right if val is higher
		error if they're the same
	*/
	if(val < (*tree)->val){
		/*
			go left
			if left exists insert to left
			else set to left
		*/
		if((*tree)->left != NULL){
			result = avl_insert(&((*tree)->left), val);
		}
		else{
			(*tree)->left = avl_new(val);
			(*tree)->left->parent = (*tree);
			result = (*tree)->left;
		}
	}
	else if(val > (*tree)->val){
		/*
			go right
			if right exists insert to right
			else set to right
		*/
		if((*tree)->right != NULL){
			result = avl_insert(&((*tree)->right), val);
		}
		else{
			(*tree)->right = avl_new(val);
			(*tree)->right->parent = (*tree);
			result = (*tree)->right;
		}
	}
	if((*tree)->parent == NULL)
		avl_balance(tree);
	avl_update_height(result);
	return result;
}

avl_tree *avl_search(avl_tree *tree, uint64_t val){
	if(val == tree->val){
		return tree;
	}
	else if(val < tree->val){
		if(tree->left){
			return avl_search(tree->left, val);
		}
		else{
			return NULL;
		}
	}
	else{
		if(tree->right){
			return avl_search(tree->right, val);
		}
		else{
			return NULL;
		}
	}
}

void avl_delete(avl_tree **tree){
	if((*tree)->left){
		avl_delete(&((*tree)->left));
	}
	if((*tree)->right){
		avl_delete(&((*tree)->right));
	}
	free(*tree);
	*tree = NULL;
}

int main(){
	avl_tree *root = avl_new(5ULL);

	avl_insert(&root, 4ULL);
	avl_insert(&root, 6ULL);
	avl_insert(&root, 7ULL);

	assert(root->height == 2);

	avl_insert(&root, 8ULL);

	assert(root->height == 3);

	avl_delete(&root);
	return 0;
}