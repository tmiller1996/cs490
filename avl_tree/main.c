/*
 * AVL-tree emperical evaluator
 *
 * main.c
 * 
 * Copyright (C) 2016 William "Amos" Confer
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "avl_tree.h"


int main() {

	uint64_t *values;
	unsigned int size;
	unsigned int i;
	unsigned int test;
	
	const unsigned int MAX_POWER = 26;
	const unsigned int CAPACITY = 1U << MAX_POWER;
	const unsigned int REPEAT_CNT = 10; 
 
	values = (uint64_t *)malloc(sizeof(uint64_t) * CAPACITY);
	if(!values) {
		fprintf(stderr, "Could not allocate values array: ");
		perror("");
		exit(EXIT_FAILURE);
	}
	
	for(size = 0; size < CAPACITY; size++) {
		values[size] = (uint64_t)size;
	}
	
	srand((unsigned int)time(NULL));
	
	/* 
	 * emperical tests proper
	 */
	for(size = 4; size <= CAPACITY; size <<= 1) {

		unsigned long elapsed = 0;

		for(test = 0; test < REPEAT_CNT; test++) {
		
			clock_t end;
			clock_t start;			
			
			avl_tree *tree = NULL;
			
			/*
			 * shuffle the values to be inserted for this test
			 */
			for(i = 0; i < size; i++) {
				uint64_t tmp = values[i];
				unsigned int j = rand() % CAPACITY;
				values[i] = values[j];
				values[j] = tmp;
			}
			
			start = clock();
			
			/*
			 * fill the tree
			 */
			for(i = 0; i < size; i++) {
				avl_insert(&tree, values[i]);
			}
			
			/*
			 * search the tree
			 */
			for(i = 0; i < size; i++) {
				avl_search(tree, values[i]);
			}
			
			avl_delete(&tree);
			
			end = clock();
			elapsed += end - start;
		}
		
		printf("size:%10u  avg. time: %10lu \n", size, elapsed / REPEAT_CNT);
		
		
		
	}
	free(values);
	
	exit(EXIT_SUCCESS);
	return 0;
}
