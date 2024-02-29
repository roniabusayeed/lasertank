#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdio.h>

/** Defines a node in a singly linekd list data structure. */
typedef struct node
{
    char** grid;
    int height;
    int width;
    struct node* next;
} node_t;

/** Inserts a copy of the map (represented by 2D array of characters)
 * pointed to by grid at the end of the linked list pointed to by list. 
 * @param list address of the pointer to the head of the linked list
 * where to insert the copy of grid. 
 * @param grid pointer to the 2D array of characters (representing a map)
 * which is to be copied into the list. 
 * @param height number of rows in the grid. 
 * @param width number of columns in the grid. */
void insert_last(node_t** list, char** grid, int height, int width);

/** Writes the contents of the linked list to a given file.
 * @param list pointer to the head of the linked list whose content
 * is to be written to the file.
 * @param filename filename of the file where the list content is
 * to be written. */
void write_list(const node_t* list, const char* filename);

/** Frees heap memory associated with list. 
 * @param list pointer to the head of the singly linked list. */
void free_list(node_t* list);

#endif  /* LINKEDLIST_H */
