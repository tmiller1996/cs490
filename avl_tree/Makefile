CC=gcc
CFLAGS=-std=c99 -Wall -Wpedantic -g -Wextra

all: main avl_test
main: avl_tree.o
avl_test: avl_tree.o
clean:
	$(RM) main avl_test *.o
