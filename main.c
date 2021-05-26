#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "utils.h"

/* Variable to keep track of player and enemy tanks. */
pos_t player_pos, enemy_pos;

/* An empty game log. */
node_t* game_log = NULL;

/* Modify this variable to adjust a preferable laser speed. */
const unsigned SLEEP_DURATION = 250U; /* In milliseconds. */

/* Entry point of the program. */
int main(int argc, char** argv)
{
    /* Declare filenames and file pointers. */
    char const* map_filename = NULL;
    const char* log_filename = NULL;
    FILE* map = NULL;
    
    /* Map dimensions. */
    int height; /* Number of rows. */
    int width;  /* Number of columns. */

    /* Pointer to the map. */
    char** grid = NULL;

    /* Flag to indicate whether or not the game should exit. */
    bool exit_flag;
    exit_flag = false;

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

    /* Read height and width of the map. */
    fscanf(map, "%d%d", &height, &width);

    /* Create map. */
    grid = create_map(height, width);

    /* Initialize the map from input file. */
    initialize_map(grid, height, width, map);

    /* Program loop. */
    while (!exit_flag)
    {
        /* Menu choice from user. */
        char menu_choice;

        /* If the player is in the line of sight of the enemy tank, the enemy
        tank fires at the player.*/
        if (in_line_of_sight(player_pos, enemy_pos, grid))
        {
            enemy_fire(&exit_flag, grid, height, width);
        }

        /* Check if exit flag is set by enemy tank. */
        if (exit_flag)
        {
            break;
        }

        /* Get menu choice from the user. */
        system("clear");
        write_map(grid, height, width, stdout);
        menu_choice = menu();

        /* Go/face up. */
        if (menu_choice == 'w')
        {
            go_or_face_upward(grid);
            
            /* Log game. */
            insert_last(&game_log, grid, height, width);
        }
        /* Go/face down. */
        else if (menu_choice == 's')
        {
            go_or_face_downward(grid, height);

            /* Log game. */
            insert_last(&game_log, grid, height, width);
        }
        /* Go/face right. */
        else if (menu_choice == 'd')
        {
            go_or_face_rightward(grid, width);

            /* Log game. */
            insert_last(&game_log, grid, height, width);
        }
        /* Go/face left. */
        else if (menu_choice == 'a')
        {
            go_or_face_leftward(grid);

            /* Log game. */
            insert_last(&game_log, grid, height, width);
        }
        /* Shoot laser. */
        else if (menu_choice == 'f')
        {
            player_fire(&exit_flag, grid, height, width);
        }
        /* Save the log. */
        else if (menu_choice == 'l')
        {
            /* Write the most recent log to the given log file. */
            write_list(game_log, log_filename);
        }
    }

    /* Free heap memory associated with map/grid. */
    delete_map(grid, height);
    grid = NULL;    /* Just to be safe. */

    /* Write the most recent log to the given log file. */
    write_list(game_log, log_filename);

    /* Free heap memory associated with game_log. */
    free_list(game_log);
    game_log = NULL;    /* Just to be safe. */

    /* Close file(s). */
    fclose(map);

    /* Success. */
    return EXIT_SUCCESS;
}
