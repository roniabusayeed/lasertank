#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/** Returns the character representation of the player for
 * a given direction of the player. */
char get_player(char dir);

/** Returns the character representation of the mirror for
 * a given direction of the mirror. */
char get_mirror(char dir);


/** Returns true if c is a character representation of a player (or enemy);
 * returns false otherwise. */
bool is_player(char c);


/** Return true if c is character representation of a mirror; returns
 * false otherwise. */
bool is_mirror(char c);


/** Returns one of the four defined direction of a player based on his/her
 * current character representation. */
char get_player_dir(char player);


/** Returns one of the two defined direction of a mirror based on its
 * current character representation. */
char get_mirror_dir(char mirror);


/** Frees heap memory associated with map (represented 
 * by 2-D array / grid of characters). 
 * @param grid pointer to the grid whose heap memory is
 * to be freed.
 * @param height number of rows in the map. */
void delete_map(char** grid, int height);


/** Displays the 2-D map on standard output. 
 * @param grid pointer to the 2-D array representing the map.
 * @param height number of rows in the map.
 * @param width number of columns in the map. */
void display_map(char** grid, int height, int width);


/** Allocates memory for a map of given height and width and
 * returns a pointer to the 2D array (essentially representing 
 * a grid).
 * @param height number of rows in the map.
 * @param width number of columns in the map. 
 * @return pointer the 2D array allocated to represent the map. */
char** create_map(int height, int width);

/** Creates a deep-copy of a 2D array (representing a map) 
 * and returns a pointer to the newly allocated memory. 
 * @param grid pointer to the 2D array representing the map.
 * @param heiht number of rows in the map.
 * @param width number of columns in the map.
 * @return pointer to the newly allocated memory/2D-array
 * representing the copy of the original map. */
char** get_copy(char** grid, int height, int width);

/** Initializes a map with from an input file. 
 * @param grid pointer to the 2D array representing the map.
 * @param height number of rows in the map.
 * @param width number of columns in the map. 
 * @param map file pointer for the input file. */
void initialize_map(char** grid, int height, int width, FILE* map);

/* Entry point of the program. */
int main(int argc, char** argv)
{
    /* Declare filenames and file pointers. */
    char const* map_filename = NULL;
    const char* log_filename = NULL;
    FILE* map = NULL;
    FILE* log = NULL;
    
    /* Map dimensions. */
    int height; /* Number of rows. */
    int width;  /* Number of columns. */

    /* Pointer to he map. */
    char** grid = NULL;

    /* Ensure proper usage. */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s %s %s\n", argv[0], "<map-filename>","<log-filename>");
        return EXIT_FAILURE;
    }

    /* Remember map filename. */
    map_filename = argv[1];
    
    /* Remember log filename. */
    log_filename = argv[2];

    /* Open map file for reading. */
    map = fopen(map_filename, "r");
    if (! map)
    {
        fprintf(stderr, "Couldn't open %s for reading.\n", map_filename);
        return EXIT_FAILURE;
    }

    /* Open log file for writing. */
    log = fopen(log_filename, "w");
    if (! log)
    {
        fprintf(stderr, "Couldn't open %s for writing.\n", log_filename);
        return EXIT_FAILURE;
    }


    /* Read height and width of the map. */
    fscanf(map, "%d%d", &height, &width);

    /* Create map. */
    grid = create_map(height, width);

    /* Initialize the map from input file. */
    initialize_map(grid, height, width, map);

    /* Display the map. */
    display_map(grid, height, width);


    /* Free memory. */
    delete_map(grid, height);

    /* Close files. */
    fclose(map);
    fclose(log);

    /* Success. */
    return EXIT_SUCCESS;
}

char** create_map(int height, int width)
{
    /* Loop counter. */
    int i;

    /* Allocate memory to store the map. */
    char** grid = malloc(sizeof(char*) * height);
    for (i = 0; i < height; i++)
    {
        grid[i] = malloc(sizeof(char) * width);
    }
    
    return grid;
}

void initialize_map(char** grid, int height, int width, FILE* map)
{
    /* Declare variable to read position and direction. */
    int x, y;
    char dir;

    /* Declare variable to read the status of file read operation. */
    int status;

    /* Loop control variables. */
    int i, j;

    /* Set the map to empty state. */
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            grid[i][j] = ' ';
        }
    }

    /* Read player data. */
    fscanf(map, "%d%d", &x, &y);
    fscanf(map, " %c", &dir);
    grid[x][y] = get_player(dir);

    /* Read enemy data. */
    fscanf(map, "%d%d", &x, &y);
    fscanf(map, " %c", &dir);
    grid[x][y] = get_player(dir);   /* As enemy tank is represented 
                                    with same symbol as the symbol of player. */
    /* As long as there are mirrors in input file. */
    while ((status = fscanf(map, "%d%d %c", &x, &y, &dir)) != EOF)
    {
        grid[x][y] = get_mirror(dir);
    }
}

void delete_map(char** grid, int height)
{
    /* Loop control variable. */
    int i;
    
    /* Iterate over the rows. */
    for (i = 0; i < height; i++)
    {
        /* Free memory for each row of the grid. */
        free(grid[i]);
    }

    /* Free memory for the grid itself.*/
    free(grid);
}

void display_map(char** grid, int height, int width)
{
    /* Loop counter variables. */
    int i, j;
    
    /* Print top border. */
    for (j = 0; j < width + 2; j++)
    {
        fprintf(stdout, "*");
    }
    fprintf(stdout, "\n");

    /* Iterate over the rows. */
    for (i = 0; i < height; i++)
    {
        fprintf(stdout, "*");
        /* Iterate over the columns. */
        for (j = 0; j < width; j++)
        {
            fprintf(stdout, "%c", grid[i][j]);
        }
        fprintf(stdout, "*\n");
    }

    /* Print bottom border. */
    for (j = 0; j < width + 2; j++)
    {
        fprintf(stdout, "*");
    }
    fprintf(stdout, "\n");
}

char get_mirror_dir(char mirror)
{
    switch (mirror)
    {
        case '/': return 'f';
        case '\\': return 'b';
        default: assert(false); /* Invalid player argument. */
    }
}

char get_player_dir(char player)
{
    switch (player)
    {
        case '<': return 'l';
        case '>': return 'r';
        case '^': return 'u';
        case 'v': return 'd';
        default: assert(false); /* Invalid player argument.*/
    }
}

bool is_mirror(char c)
{
    switch (c)
    {
        case '/':
        case '\\': return true;
        default: return false;
    }
}

bool is_player(char c)
{
    switch (c)
    {
        case '<':
        case '>':
        case '^':
        case 'v': return true;
        default: return false;
    }
}

char get_mirror(char dir)
{
    switch (dir)
    {
        case 'f': return '/';
        case 'b': return '\\';
        default: assert(false); /* Invalid direction argument. */
    }
}

char get_player(char dir)
{
    switch (dir)
    {
        case 'u': return '^';   /* Up. */
        case 'd': return 'v';   /* Down. */
        case 'r': return '>';   /* Right. */
        case 'l': return '<';   /* Left. */
        default: assert(false); /* Invalid direction argument. */
    }
}
