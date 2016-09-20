#include "avl_tree.h"
#include <stdlib.h>
#include <stdio.h>

void avl_print(avl_tree*, int);

int main(int argc, char **argv){
	avl_tree *root = NULL;
	int i;
	for(i = 1; i < argc; i++){
		uint64_t val = (uint64_t) atoi(argv[i]);
		avl_insert(&root, val);
	}

	avl_print(root, 0);

	avl_delete(&root);
}