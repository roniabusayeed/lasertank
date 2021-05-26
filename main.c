#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "sleep.h"
#include "colors.h"
#include "utils.h"

/* Variable to keep track of player and enemy tanks. */
pos_t player_pos, enemy_pos;

/* An empty game log. */
node_t* game_log = NULL;

/** Returns true if the player is in the line of sight of the enemy tank.
 * return false otherwise. */
bool in_line_of_sight(pos_t player_pos, pos_t enemy_pos, char** grid)
{
    if (player_pos.x == enemy_pos.x)
    {
        /* Player is in the horizontal line of sight of the enemy tank. */
        char enemy_dir;
        enemy_dir = get_player_dir(grid[enemy_pos.x][enemy_pos.y]);
        if ((enemy_dir == 'r' && enemy_pos.y < player_pos.y) ||
        (enemy_dir == 'l' && player_pos.y < enemy_pos.y))
        {
            /* Enemy fires. */
            return true;
        }
    }
    else if (player_pos.y == enemy_pos.y)
    {
        /* Player is in the vertical line of sight of the enemy tank. */
        char enemy_dir;
        enemy_dir = get_player_dir(grid[enemy_pos.x][enemy_pos.y]);
        if ((enemy_dir == 'u' && player_pos.x < enemy_pos.x) ||
        (enemy_dir == 'd' && enemy_pos.x < player_pos.x))
        {
            /* Enemy fires. */
            return true;
        }
    }
    return false;
}

/** Attemps to make the player face to go one step upward. */
void go_or_face_upward(char** grid)
{
    extern pos_t player_pos;

    /* If the player is not already facing upward. */
    if (get_player_dir(grid[player_pos.x][player_pos.y]) != 'u')
    {
        /* Face upward. */
        grid[player_pos.x][player_pos.y] = get_player('u');
    }
    /* Attemp to move one step upward. */
    else
    {
        /* The player cannot go to the same position as that of the mirror. */
        /* The player cannot go out of the boundary of the map. */
        if (player_pos.x - 1 >= 0 && 
        !is_mirror(grid[player_pos.x - 1][player_pos.y]) &&
        !is_player(grid[player_pos.x - 1][player_pos.y]))
        {
            /* Move the player one step upward. */
            grid[player_pos.x - 1][player_pos.y] = grid[player_pos.x][player_pos.y];
            grid[player_pos.x][player_pos.y] = ' ';
            player_pos.x--;
        }
    }
}

/** Attemps to make the player face to go one step upward. */
void go_or_face_downward(char** grid, int height)
{
    extern pos_t player_pos;

    /* If the player is not already facing downward. */
    if (get_player_dir(grid[player_pos.x][player_pos.y]) != 'd')
    {
        /* Face downward. */
        grid[player_pos.x][player_pos.y] = get_player('d');
    }
    /* Attemp to move one step downward. */
    else
    {
        /* The player cannot go to the same position as that of the mirror. */
        /* The player cannot go out of the boundary of the map. */
        if (player_pos.x + 1 < height &&
        !is_mirror(grid[player_pos.x + 1][player_pos.y]) &&
        !is_player(grid[player_pos.x + 1][player_pos.y]))
        {
            /* Move the player one step downward. */
            grid[player_pos.x + 1][player_pos.y] = grid[player_pos.x][player_pos.y];
            grid[player_pos.x][player_pos.y] = ' ';
            player_pos.x++;
        }
    }
}

/** Attemps to make the player face to go one step rightward. */
void go_or_face_rightward(char** grid, int width)
{
    extern pos_t player_pos;

    /* If the player is not already facing rightward. */
    if (get_player_dir(grid[player_pos.x][player_pos.y]) != 'r')
    {
        /* Face rightward. */
        grid[player_pos.x][player_pos.y] = get_player('r');
    }
    /* Attemp to move one step rightward. */
    else
    {
        /* The player cannot go to the same position as that of the mirror. */
        /* The player cannot go out of the boundary of the map. */
        if (player_pos.y + 1 < width &&
        !is_mirror(grid[player_pos.x][player_pos.y + 1]) &&
        !is_player(grid[player_pos.x][player_pos.y + 1]))
        {
            /* Move the player one step rightward. */
            grid[player_pos.x][player_pos.y + 1] = grid[player_pos.x][player_pos.y];
            grid[player_pos.x][player_pos.y] = ' ';
            player_pos.y++;
        }
    }
}

/** Attemps to make the player face to go one step leftward. */
void go_or_face_leftward(char** grid)
{
    extern pos_t player_pos;

    /* If the player is not already facing leftward. */
    if (get_player_dir(grid[player_pos.x][player_pos.y]) != 'l')
    {
        /* Face leftward. */
        grid[player_pos.x][player_pos.y] = get_player('l');
    }
    /* Attemp to move one step leftward. */
    else
    {
        /* The player cannot go to the same position as that of the mirror. */
        /* The player cannot go out of the boundary of the map. */
        if (player_pos.y - 1 >= 0 &&
        !is_mirror(grid[player_pos.x][player_pos.y - 1]) && 
        !is_player(grid[player_pos.x][player_pos.y - 1]))
        {
            /* Move the player one step leftward. */
            grid[player_pos.x][player_pos.y - 1] = grid[player_pos.x][player_pos.y];
            grid[player_pos.x][player_pos.y] = ' ';
            player_pos.y--;
        }
    }
}


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
            /* Laser vector. */
            pos_t laser_pos;
            char laser_dir;

            /* Initial position of the laser. */
            laser_pos.x = player_pos.x;
            laser_pos.y = player_pos.y;

            /* Initial direction of the laser. */
            laser_dir = get_player_dir(grid[player_pos.x][player_pos.y]);

            /* Laser loop. */
            while (true)
            {
                /* Advance. */
                if (laser_dir == 'u')
                {
                    laser_pos.x--;
                }
                else if (laser_dir == 'd')
                {
                    laser_pos.x++;
                }
                else if (laser_dir == 'l')
                {
                    laser_pos.y--;
                }
                else if (laser_dir == 'r')
                {
                    laser_pos.y++;
                }

                /* If laser is out of bounds, break. */
                if (laser_pos.x < 0 || laser_pos.x >= height 
                || laser_pos.y < 0 || laser_pos.y >= width)
                {
                    break;
                }

                /* If laser hits the mirror, change direction. */
                if (is_mirror(grid[laser_pos.x][laser_pos.y]))
                {
                    /* Get mirror direction: (forward or backward slash) */
                    char mirror_dir;
                    mirror_dir = get_mirror_dir(grid[laser_pos.x][laser_pos.y]);

                    /* Forward mirror. */
                    if (mirror_dir == 'f')
                    {
                        /* Change final laser direction based on its initial direction. */
                        if (laser_dir == 'd') laser_dir = 'l';
                        else if (laser_dir == 'u') laser_dir = 'r';
                        else if (laser_dir == 'r') laser_dir = 'u';
                        else if (laser_dir == 'l') laser_dir = 'd';
                    }
                    /* Backward mirror. */
                    else if (mirror_dir == 'b')
                    {
                        if (laser_dir == 'd') laser_dir = 'r';
                        else if (laser_dir == 'u') laser_dir = 'l';
                        else if (laser_dir == 'r') laser_dir = 'd';
                        else if (laser_dir == 'l') laser_dir = 'u';
                    }
                }

                /* If laser hits the enemy tank, declare win and exit. */
                else if (laser_pos.x == enemy_pos.x && laser_pos.y == enemy_pos.y)
                {
                    fprintf(stdout, "You win!\n");
                    exit_flag = true;
                    break;
                }
                else
                {
                    /* The laser is inside the grid, it didn't hit a mirror, and
                    and it didn't hit an enemy tank. */

                    /* Print laser beam. */
                    if (laser_dir == 'u' || laser_dir == 'd')
                    {
                        grid[laser_pos.x][laser_pos.y] = '|';
                    }
                    else
                    {
                        grid[laser_pos.x][laser_pos.y] = '-';
                    }
                    system("clear");
                    write_map(grid, height, width, stdout);
                    
                    /* Log game. */
                    insert_last(&game_log, grid, height, width);
                    
                    msleep(500);    /* Sleep for 500 ms. */
                    grid[laser_pos.x][laser_pos.y] = ' ';
                }
            }
        }
        /* Save the log. */
        else if (menu_choice == 'l')
        {
            /* Write the most recent log to the given log file. */
            write_list(game_log, log_filename);
        }
    }

    /* Free memory associated with map/grid. */
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
