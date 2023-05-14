#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/*
INTRO:
For createMaze, I opened the passed file in read mode, and scanned the row and column dimensions into variables. I then allocated memory
for the maze struct, assigned the width and height values for the maze, and allocated memory and assigned the cells attribute of the maze (which acts
like a bunch of row pointers). After this, I allocated memory for the cells using each row pointer, and assigned and scanned the values for the cells into the newly allocated
spots, skipping newlines and assigning the start and end values in the maze struct accordingly. All of this is to correctly initialize the maze, a
pointer to which is then returned. For destroyMaze, I started by freeing the cells of the maze using the first "row pointer", as this is the only
"row pointer" that can free every cell, before freeing the row pointers and then the maze itself. For printMaze, I started by changing the start position
back into 'S' in memory, since it gets changed into an '*' while solving, before iterating through the rows and columns of the maze, printing every entry
and printing a newline after every row. Finally, for solveMazeDFS, I implemented the algorithm given on the wiki; returning false if the position is out
of the bounds of the maze, returning false if the position isn't empty, the start, or the end, returning true if the position is the end, and then
recursively calling the function on the cells to the right, below, to the left, and above the current cell, returning 1 if any of these lead to a solution.
If all of these fail, we return 0 to indicate a solution can't be reached.
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    FILE *in_file = fopen(fileName, "r"); //open passed file in reading mode
    if(in_file == NULL) {
        return NULL; //if opening file fails, return NULL
    }
    int row, col; //declare variables for row and column of maze
    fscanf(in_file, "%d %d", &row, &col); //scan row and column dimensions from file into row and column variables
    maze_t *maze_pointer = (maze_t *)malloc(sizeof(maze_t)); //allocate memory for maze, store pointer for maze into variable
    maze_pointer->height = row; //assign height of maze
    maze_pointer->width = col; //assign width of maze
    maze_pointer->cells = (char **)malloc(row*sizeof(char*)); //allocate memory for rows of maze, which will point to individual column entries
    for (int i = 0; i < row; i++) {
        *(maze_pointer->cells + i) = (char *)malloc(col*sizeof(char)); //allocate memory for cells using row pointers
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fscanf(in_file, "%c", *(maze_pointer->cells + i) + j); //scan characters from file into allocated memory for cells
            if (maze_pointer->cells[i][j] == 10) { //if character we've scanned is newline, we want to scan next character into memory
                fscanf(in_file, "%c", *(maze_pointer->cells + i) + j);
            }
            if (maze_pointer->cells[i][j] == 'S') { //if character we've scanned is start, assign values to our maze struct accordingly
                maze_pointer->startRow = i;
                maze_pointer->startColumn = j;
            }
            if (maze_pointer->cells[i][j] == 'E') { //if character we've scanned is end, assign values to our maze struct accordingly
                maze_pointer->endRow = i;
                maze_pointer->endColumn = j;
            }
        }
    }
    fclose(in_file);
    return maze_pointer;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    for (int i = 0; i < maze->height; i++) {
        free(maze->cells[i]); //free every cell in maze
    }
    //free(maze->cells[0]); //free every maze cell we've populated
    free(maze->cells); //free the maze row pointers
    free(maze); //free the maze itself
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    maze->cells[maze->startRow][maze->startColumn] = 'S'; //this gets marked with an asterisk in the solution, so we need to change it back
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            printf("%c", maze->cells[i][j]); //print every entry in maze
        }
        printf("\n"); //print newline after every row
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    if (col >= maze->width || col < 0 || row >= maze->height || row < 0) { //if position is out of bounds, return false
        return 0;
    }
    if (maze->cells[row][col] != ' ' && maze->cells[row][col] != 'S' && maze->cells[row][col] != 'E') { //if position isn't empty or end/start, return false
        return 0;
    }
    if (maze->cells[row][col] == 'E') { //if position is the end of the maze, return true
        return 1;
    }
    maze->cells[row][col] = '*'; //mark position as part of solution path of maze
    if (solveMazeDFS(maze, col - 1, row)) { //if position left of current is part of solution path, return true
        return 1;
    }
    if (solveMazeDFS(maze, col + 1, row)) { //if position right of current is part of solution path, return true
        return 1;
    }
    if (solveMazeDFS(maze, col, row - 1)) { //if position above current is part of solution path, return true
        return 1;
    }
    if (solveMazeDFS(maze, col, row + 1)) { //if position below current is part of solution path, return true
        return 1;
    }
    maze->cells[row][col] = '~'; //if current isn't part of solution path, mark it as visited but not solution
    return 0; //return false if nothing around current position leads to solution
}
