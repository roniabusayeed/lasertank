#include "linkedlist.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/** External functions called by linked list api. */
extern void delete_map(char** grid, int height);
extern void write_map(char** grid, int height, int width, FILE* stream);
extern char** get_copy(char** grid, int height, int width);

void insert_last(node_t** list, char** grid, int height, int width)
{
    char** new_grid;
    node_t* new_node;
    node_t* last;

    /* Copy map. */
    new_grid = get_copy(grid, height, width);
    
    /* Allocate a new node and store the new grid into
    the new node. */
    new_node = malloc(sizeof(node_t));
    new_node->grid = new_grid;
    new_node->height = height;
    new_node->width = width;
    new_node->next = NULL;

    /* Insert the new node at the end of the linked list. */
    last = *list;
    if (*list == NULL)
    {
        *list = new_node;
        return;
    }

    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = new_node;
    return;
}

void write_list(const node_t* list, const char* filename)
{
    /* Flag indicating whether or not a map is written. */
    bool first_map_written;
    const node_t* current;

    /* Open file for reading. */
    FILE* outfile;
    outfile = fopen(filename, "w");
    if (! outfile)
    {
        fprintf(stderr, "Couldn't open %s for writing.\n", filename);
        return;
    }

    /* Traverse the linked list. */
    first_map_written = false;
    for (current = list; current != NULL; current = current->next)
    {
        /* Print a separator line in between each snapshot of map. */
        if (first_map_written)
        {
            int i;
            fprintf(outfile,"\n");
            for (i = 0; i < current->width + 2; i++)
            {
                fprintf(outfile, "-");
            }
            fprintf(outfile, "\n\n");
        }

        /* Write the map to file pointed to by outfile pointer. */
        write_map(current->grid, current->height, current->width, outfile);
        first_map_written = true;
    }

    /* Close file. */
    fclose(outfile);
}

void free_list(node_t* list)
{
    node_t* next;
    while (list != NULL)
    {
        next = list->next;
        delete_map(list->grid, list->height);
        free(list);
        list = next;
    }
}
