#include "game.h"

/*
INTRO: for make_game, I initialized every element of the game struct to the desired value: rows to the given row size, cols to the given column size, each cell to -1 since the board starts
off empty, the score to 0, and returning a pointer to the game. For remake_game I did the same thing using the provided game pointer. For get_cell I first checked that the cell was within the
bounds of the board. If it was, I returned a pointer to the correct cell using the correct offset from the cells element of the game struct. For move_w, I first declared an array of integers the
same size of the board to represent if a particular cell had been combined at already for a given move, since double combinations of the same tile aren't allowed in 2048. After this, I iterated
through every cell in the board (left to right, top to bottom), checking to see if it could be combined with any tiles above it, and if so combining it and marking the spot as combined. After
this, I identified the highest row each tile could be moved into and moved it there. If any tiles moved or combined, I set the previously declared variable change to 1, indicating that a change
had occurred and the move should be processed. For move_a, I used the same algorithm as move_w but iterated right to left, bottom to top. For move_a, I used the same algorithm as move_w, but
iterated top to bottom, left to right. For move_d, I used the same algorithm as move_w, but iterated bottom to top, right to left. For legel_move_check, I created a deep copy of the current game,
(so the check doesn't modify the actual game board) and tried all possible moves. If any move caused a change, I set a variable change to 1 to indicate some change had occurred. If no change
occurred, there are no possible moves left, so the game is over.
*/


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->rows = rows; //initialize rows of game
    mygame->cols = cols; //initialize columns of game

    mygame->score = 0; //initialize score variable to zero

    cell *cell_ptr = mygame->cells;

    for (int i = 0; i < (rows*cols); i++) {
        *(cell_ptr + i) = -1; //initialize every cell to -1
    }

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows; //initialize rows of game
    (*_cur_game_ptr)->cols = new_cols; //initialize columns of game

    (*_cur_game_ptr)->score = 0; //initialize score variable to zero

    cell *cell_ptr = (*_cur_game_ptr)->cells; //variable for pointer to cells for convenience

    for (int i = 0; i < (new_rows*new_cols); i++) {
        *(cell_ptr + i) = -1; //initialize every cell to -1
    }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    if (row >= 0 && col >= 0 && row < cur_game->rows && col < cur_game->cols) {
        cell *return_val; //declare our cell pointer we will return
        return_val = (cur_game->cells + ((row * cur_game->cols) + col)); //set return_val to the correct address
        return return_val; //return the pointer
    }

    return NULL; //if requested cell is out of bounds return NULL
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    int combined[cur_game->rows * cur_game->cols]; //create an array to indicate whether a particular cell has already been combined at for a move or not
    for (int i = 0; i < cur_game->rows * cur_game->cols; i++) {
        combined[i] = 0; //initialize every spot in the array to 0 for uncombined
    }
    int change = 0; //initialize variable to indicate if board has changed to 0

    for (int i = 0; i < cur_game->rows; i++) { //iterate through every row in game board
        for (int j = 0; j < cur_game->cols; j++) { //iterate through every column in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if cell is empty, we can move on
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int row = 0; row < i; row++) { //figure out the lowest index row we can move the current cell into
                    int blocking = 0; //declare variable to represent whether or not a combination is blocked by a different tile (i.e. in 2 8 2 the 8 is blocking the 2s from combining)
                    cell *combo_cell = get_cell(cur_game, row, j); //pointer to potential cell current cell can combine with
                    if (*combo_cell == *current_cell && !combined[(((cur_game->cols) * row) + j)]) { //if the values of the tiles are equal and cell hasn't been combined onto already
                        for (int check_row = row + 1; check_row < i; check_row++) { //check through column to make sure combination isn't blocked by any other tiles
                            if (*get_cell(cur_game, check_row, j) >= 0) {
                                blocking = 1;
                            }
                        }
                        if (!blocking) { //if above criteria are met and combination isn't blocked, combination is successful
                            *combo_cell = (*current_cell) * 2; //since this is a combination, double the value at the spot we are moving to
                            cur_game->score = cur_game->score + *combo_cell; //add the correct value to the score
                            *current_cell = -1; //empty the current cell, since it has now been combined
                            combined[(((cur_game->cols) * row) + j)] = 1; //indicate that the cell has been combined, so it can't be combined again
                            change = 1; //indicate that a change has occurred and the move is valid
                            break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < cur_game->rows; i++) { //iterate through every row in game board
        for (int j = 0; j < cur_game->cols; j++) { //iterate through every column in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int row = 0; row < i; row++) { //figure out the lowest index row we can move the current cell into
                    if (*get_cell(cur_game, row, j) < 0) { //if spot in board is empty
                        *get_cell(cur_game, row, j) = *current_cell; //assign value of current spot to empty spot
                        *current_cell = -1; //empty current spot
                        change = 1; //indicate that a change has occurred so the move is valid
                        break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                    }
                }
            }
        }
    }

    if (!change) { //if no change has occurred, move shouldn' be processed
        return 0;
    }

    return 1;
};

int move_s(game * cur_game) //slide down
{
    int combined[cur_game->rows * cur_game->cols]; //create an array to indicate whether a particular cell has already been combined at for a move or not
    for (int i = 0; i < cur_game->rows * cur_game->cols; i++) {
        combined[i] = 0; //initialize every spot in the array to 0 for uncombined
    }
    int change = 0; //initialize variable to indicate if board has changed to 0

    for (int i = cur_game->rows - 1; i >= 0; i--) { //iterate through every row in game board in reverse this time since we're moving down
        for (int j = cur_game->cols - 1; j >= 0; j--) { //iterate through every column in game board in reverse, though it shouldn't actually matter
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int row = cur_game->rows - 1; row > i; row--) { //figure out the highest index row we can move the current cell into
                    int blocking = 0; //declare variable to represent whether or not a combination is blocked by a different tile (i.e. in 2 8 2 the 8 is blocking the 2s from combining)
                    cell *combo_cell = get_cell(cur_game, row, j); //pointer to potential cell current_cell can combine with
                    if (*combo_cell == *current_cell && !combined[(((cur_game->cols) * row) + j)]) { //if the values of the tiles are equal and cell hasn't been combined onto already
                        for (int check_row = row - 1; check_row > i; check_row--) { //check to see if any tiles in column are blocking combination
                            if (*get_cell(cur_game, check_row, j) >= 0) {
                                blocking = 1;
                            }
                        }
                        if (!blocking) { //if combination isn't blocked
                            *combo_cell = (*current_cell) * 2; //since this is a combination, double the value at the spot we are moving to
                            *current_cell = -1; //empty the current cell, since it has now been combined
                            cur_game->score = cur_game->score + *combo_cell; //add the correct value to the score
                            combined[(((cur_game->cols) * row) + j)] = 1; //indicate that the cell has been combined, so it can't be combined again
                            change = 1; //indicate that a change has occurred and the move is valid
                            break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                        }
                    }
                }
            }
        }
    }
    
    for (int i = cur_game->rows - 1; i >= 0; i--) { //iterate through every row in game board (again in reverse)
        for (int j = cur_game->cols - 1; j >= 0; j--) { //iterate through every column in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int row = cur_game->rows - 1; row > i; row--) { //figure out the lowest index row we can move the current cell into
                    if (*get_cell(cur_game, row, j) < 0) { //if spot in board is empty
                        *get_cell(cur_game, row, j) = *current_cell; //assign value of current spot to empty spot
                        *current_cell = -1; //empty current spot
                        change = 1; //indicate that a change has occurred so the move is valid
                        break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                    }
                }
            }
        }
    }

    if (!change) { //if no change has occurred, move shouldn't be processed
        return 0;
    }

    return 1;
};

int move_a(game * cur_game) //slide left
{
    int combined[cur_game->rows * cur_game->cols]; //create an array to indicate whether a particular cell has already been combined at for a move or not
    for (int i = 0; i < cur_game->rows * cur_game->cols; i++) {
        combined[i] = 0; //initialize every spot in the array to 0 for uncombined
    }
    int change = 0; //initialize variable to indicate if board has changed to 0

    for (int j = 0; j < cur_game->cols; j++) { //iterate through every column in game board
        for (int i = 0; i < cur_game->rows; i++) { //iterate through every row in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int col = 0;  col < j; col++) { //figure out the lowest index row we can move the current cell into
                    int blocking = 0; //declare variable to represent whether or not a combination is blocked by a different tile (i.e. in 2 8 2 the 8 is blocking the 2s from combining)
                    cell *combo_cell = get_cell(cur_game, i, col); //pointer to potential cell current_cell can combine with
                    if (*combo_cell == *current_cell && !combined[(((cur_game->cols) * i) + col)]) { //if the values of the tiles are equal and cell hasn't been combined onto already
                        for (int check_col = col + 1; check_col < j; check_col++) { //check if any tiles are blocking the combination
                            if (*get_cell(cur_game, i, check_col) >= 0) {
                                blocking = 1;
                            }
                        }
                        if (!blocking) {
                            *combo_cell = (*current_cell) * 2; //since this is a combination, double the value at the spot we are moving to
                            *current_cell = -1; //empty the current cell, since it has now been combined
                            cur_game->score = cur_game->score + *combo_cell; //add the correct value to the score
                            combined[(((cur_game->cols) * i) + col)] = 1; //indicate that the cell has been combined, so it can't be combined again
                            change = 1; //indicate that a change has occurred and the move is valid
                            break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                        }
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < cur_game->cols; j++) { //iterate through every column in game board
        for (int i = 0; i < cur_game->rows; i++) { //iterate through every row in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int col = 0;  col < j; col++) { //figure out the lowest index row we can move the current cell into
                    if (*get_cell(cur_game, i, col) < 0) { //if spot in board is empty
                        *get_cell(cur_game, i, col) = *current_cell; //assign value of current spot to empty spot
                        *current_cell = -1; //empty current spot
                        change = 1; //indicate that a change has occurred so the move is valid
                        break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                    }
                }
            }
        }
    }


    if (!change) { //if no change has occurred, move shouldn't be processed
        return 0;
    }

    return 1;
};

int move_d(game * cur_game){ //slide to the right
    
    int combined[cur_game->rows * cur_game->cols]; //create an array to indicate whether a particular cell has already been combined at for a move or not
    for (int i = 0; i < cur_game->rows * cur_game->cols; i++) {
        combined[i] = 0; //initialize every spot in the array to 0 for uncombined
    }
    int change = 0; //initialize variable to indicate if board has changed to 0

    for (int j = cur_game->cols - 1; j >= 0; j--) { //iterate through every column in game board (in reverse this time since we're sliding right)
        for (int i = cur_game->rows - 1; i >= 0; i--) { //iterate through every row in game board (also in reverse, though it shouldn't matter)
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int col = cur_game->cols - 1;  col > j; col--) { //figure out the lowest index row we can move the current cell into
                    int blocking = 0; //declare variable to represent whether or not a combination is blocked by a different tile (i.e. in 2 8 2 the 8 is blocking the 2s from combining)
                    cell *combo_cell = get_cell(cur_game, i, col); //pointer to potential cell current_cell can combine with
                    if (*combo_cell == *current_cell && !combined[(((cur_game->cols) * i) + col)]) { //if the values of the tiles are equal and cell hasn't been combined onto already
                        for (int check_col = col - 1; check_col > j; check_col--) { //make sure there isn't any blockage going on
                            if (*get_cell(cur_game, i, check_col) >= 0) {
                                blocking = 1;
                            }
                        }
                        if (!blocking) {
                            *combo_cell = (*current_cell) * 2; //since this is a combination, double the value at the spot we are moving to
                            *current_cell = -1; //empty the current cell, since it has now been combined
                            cur_game->score = cur_game->score + *combo_cell; //add the correct value to the score
                            combined[(((cur_game->cols) * i) + col)] = 1; //indicate that the cell has been combined, so it can't be combined again
                            change = 1; //indicate that a change has occurred and the move is valid
                            break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                        }
                    }
                }
            }
        }
    }
    
    for (int j = cur_game->cols - 1; j >= 0; j--) { //iterate through every column in game board (in reverse)
        for (int i = cur_game->rows - 1; i >= 0; i--) { //iterate through every row in game board
            if (*get_cell(cur_game, i, j) >= 0) { //if current cell isn't empty
                cell *current_cell = get_cell(cur_game, i, j); //assign a variable for the pointer to the current cell to make code easier to read
                for (int col = cur_game->cols - 1;  col > j; col--) { //figure out the lowest index row we can move the current cell into
                    if (*get_cell(cur_game, i, col) < 0) { //if spot in board is empty
                        *get_cell(cur_game, i, col) = *current_cell; //assign value of current spot to empty spot
                        *current_cell = -1; //empty current spot
                        change = 1; //indicate that a change has occurred so the move is valid
                        break; //don't want to keep checking and modifying cells if we've already found the lowest index row we can move to
                    }
                }
            }
        }
    }

    if (!change) { //if no change has occurred, move shouldn't be processed
        return 0;
    }

    return 1;
};

int legal_move_check(game * cur_game)

/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    int change = 0; //declare variable to determine if board will change or not
    game * game_copy = make_game(cur_game->rows, cur_game->cols); //pointer to new game
    cell * copy_ptr = game_copy->cells; //pointer to copy's cells
    cell * cur_ptr = cur_game->cells; //pointer to original's cells
    for (int n = 0; n < cur_game->rows * cur_game->cols; n++) {
        *(copy_ptr + n) = *(cur_ptr + n); //copy all of the current game's values over to the memory of the game copy so we don't modify the current game
    }
    change = move_w(game_copy);         //
    if (!change) {                      //
        change = move_s(game_copy);     //
    }                                   //
    if (!change) {                      //------>check all movement directions until we get one that is possible, because in that case there is at least one legal move
        change = move_a(game_copy);     //
    }                                   //
    if (!change) {                      //
        change = move_d(game_copy);     //
    }                                   //

    destroy_game(game_copy); //free memory associated with the copy of the game board.
    
    if (!change) { //if no movement directions are possible, return 0
        return 0;
    }

    return 1;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:   printf("fuck");
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
    

    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
