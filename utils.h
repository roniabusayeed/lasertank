#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdbool.h>

/** Defines a (x,y) position in 2-D map. */
typedef struct
{
    int x, y;
} pos_t;

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

/** Writes a map to the a file stream. 
 * @param grid pointer to the 2D array of characters representing a map.
 * @param height number of rows in the grid.
 * @param width number of columns in the grid.
 * @param stream file stream where the map is to be written. */
void write_map(char** grid, int height, int width, FILE* stream);

/** This function is called when the player is in the line of sight
 * of the enemy tank. 
 * @param exit_flag exit flag of the program.
 * @param grid pointer to the 2D array of character representing map.
 * @param height number of rows in the grid.
 * @param width number of columns in the grid. /*/
void enemy_fire(bool* exit_flag, char** grid, int height, int width);

/** This function is called when the player is in the line of sight
 * of the enemy tank. 
 * @param exit_flag exit flag of the program.
 * @param grid pointer to the 2D array of character representing map.
 * @param height number of rows in the grid.
 * @param width number of columns in the grid. /*/
void player_fire(bool* exit_flag, char** grid, int height, int width);

/** Returns true if the player is in the line of sight of the enemy tank.
 * return false otherwise. 
 * @param player_pos position of the player tank.
 * @param enemy_pos position of the enemy tank.
 * @param grid pointer to the 2D array of characters representing the map.
 * @return true if player is in line of sight of the enemy tank,
 * returns false otherwise.*/
bool in_line_of_sight(pos_t player_pos, pos_t enemy_pos, char** grid);

/** Attemps to make the player face to go one step upward. 
 * @param grid pointer to the 2D array of characters representing the map. */
void go_or_face_upward(char** grid);

/** Attemps to make the player face to go one step upward. 
 * @param grid pointer to the 2D array of characters representing map.
 * @param height number of rows in the grid. */
void go_or_face_downward(char** grid, int height);

/** Attemps to make the player face to go one step rightward.
 * @param grid pointer to the 2D array of characters representing the map. 
 * @param width number of columns in the grid. */
void go_or_face_rightward(char** grid, int width);

/** Attemps to make the player face to go one step leftward. 
 * @param grid pointer to the 2D array of characters representing the map. */
void go_or_face_leftward(char** grid);

/** Gets a valid menu choice from the user. 
 * @return character representation of the menu choice.
 * 
 * w: to move upward.
 * a: to move leftward.
 * s: to move downward.
 * d: to move rightward.
 * f: to fire.
 * l: to save game log. */
char menu();

#endif  /* UTILS_H */