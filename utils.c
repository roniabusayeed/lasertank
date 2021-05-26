#include "utils.h"
#include <stdbool.h>
#include "linkedlist.h"
#include <stdlib.h>
#include <assert.h>
#include "colors.h"
#include "sleep.h"

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
    extern pos_t player_pos;
    extern pos_t enemy_pos;

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
    player_pos.x = x;
    player_pos.y = y;

    /* Read enemy data. */
    fscanf(map, "%d%d", &x, &y);
    fscanf(map, " %c", &dir);
    grid[x][y] = get_player(dir);   /* As enemy tank is represented 
                                    with same symbol as the symbol of player. */
    enemy_pos.x = x;
    enemy_pos.y = y;

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

char** get_copy(char** grid, int height, int width)
{
    /* Loop control variable. */
    int i, j;

    /* Allocate memory for new map/grid. */
    char** new_grid = malloc(sizeof(char*) * height);
    for (i = 0; i < height; i++)
    {
        new_grid[i] = malloc(sizeof(char) * width);
    }

    /* Copy each grid cell over to the new grid. */
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            new_grid[i][j] = grid[i][j];
        }
    }
    
    /* Return copy. */
    return new_grid;
}

char menu()
{
    char choice;
    do {
        fprintf(stdout, "w to go/face up\n");
        fprintf(stdout, "s to go/face down\n");
        fprintf(stdout, "a to go/face left\n");
        fprintf(stdout, "d to go/face right\n");
        fprintf(stdout, "f to shoot laser\n");
        fprintf(stdout, "l to save the log\n");
        fprintf(stdout, "action: ");
        fscanf(stdin, " %c", &choice);
    } while (choice != 'w' && choice != 's' && choice != 'a' && choice != 'd'
    && choice != 'f' && choice != 'l');
    return choice;
}

void write_map(char** grid, int height, int width, FILE* stream)
{
    /* Loop counter variables. */
    int i, j;
    
    /* Print top border. */
    for (j = 0; j < width + 2; j++)
    {
        fprintf(stream, "*");
    }
    fprintf(stream, "\n");

    /* Iterate over the rows. */
    for (i = 0; i < height; i++)
    {
        fprintf(stream, "*");
        /* Iterate over the columns. */
        for (j = 0; j < width; j++)
        {
            char grid_cell;
            grid_cell = grid[i][j];

            /* If the grid cell has a laser beam, write it with color. */
            if (grid_cell == '|' || grid_cell == '-')
            {
                /* Print colored output only when in terminal. */
                if (stream == stdout || stream == stderr)
                {
                    fprintf (stream, FRED("%c"), grid_cell);
                }
                else
                {
                    fprintf (stream, "%c", grid_cell);
                }
            }
            else
            {
                fprintf(stream, "%c", grid_cell);
            }
        }
        fprintf(stream, "*\n");
    }

    /* Print bottom border. */
    for (j = 0; j < width + 2; j++)
    {
        fprintf(stream, "*");
    }
    fprintf(stream, "\n");
}

void enemy_fire(bool* exit_flag, char** grid, int height, int width)
{
    extern node_t* game_log;
    extern pos_t enemy_pos;
    extern pos_t player_pos;

    /* Laser vector. */
    pos_t laser_pos;
    char laser_dir;

    /* Initial position of the laser. */
    laser_pos.x = enemy_pos.x;
    laser_pos.y = enemy_pos.y;

    /* Initial direction of the laser. */
    laser_dir = get_player_dir(grid[enemy_pos.x][enemy_pos.y]);

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

        /* If laser hits the player tank, declare lose and exit. */
        else if (laser_pos.x == player_pos.x && laser_pos.y == player_pos.y)
        {
            fprintf(stdout, "You lose!\n");
            *exit_flag = true;
            break;
        }
        else
        {
            /* The laser is inside the grid, it didn't hit a mirror, and
            and it didn't hit a player tank. */

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

void player_fire(bool* exit_flag, char** grid, int height, int width)
{
    extern node_t* game_log;
    extern pos_t enemy_pos;
    extern pos_t player_pos;

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
            *exit_flag = true;
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
