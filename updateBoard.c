/*
INTRO: 
countLiveNeighbor: I implemented the algorithm provided on the wiki exactly; loop through the neighbor rows for a cell, 
make sure each row is in bounds, loop through the neighbor columns for each row, make sure each column is in bounds, then make sure
that we aren't checking the cell whose neighbors we are analyzing, and then check if the cell (at index [r*boardColSize + c] since
the board is a 1D array) is alive. If so, increment the count of live neighbors.

generateNextStep: For use in updateBoard and aliveStable. This function takes a pointer to an array for the next board as input, and populates this array
with the next board, using countLiveNeighbor to determine if a cell will die, and if not that if it will come back to life, and if
not that matching it with the previous board.

updateBoard: For this function, I first declare a new array of the same size as the board to hold the next board. I then call generateNextStep to populate
the next board, and then fill the actual board with the data in the next board.

aliveStable: For this function, I first declare a new array of the same size as the board to hold the next board. I then call generateNextStep to populate
the next board, and then compare the actual board with the data in the next board. If there is any difference at any position on the board, return 0, otherwise
the data is the same and 1 is returned.
*/

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col) {
    int live_neighbor_count; //variable to represent number of live neighbors for given cell
    live_neighbor_count = 0; //initialize count with 0 live neighbors
    for (int r = row - 1; r <= row + 1; r++) { //loop through rows around current cell
        if (r >= 0 && r < boardRowSize ) { //check that row we are checking is within the bounds of the board
            for (int c = col - 1; c <= col + 1; c++) { //loop through columns in each row
                if (c >= 0 && c < boardColSize) { //make sure that the column we are checking is within the bounds of the board
                    if (r != row || c != col) { //make sure we aren't including the cell we are currently determining
                        if (board[r*boardColSize + c]) { //if neighbor cell is alive
                            live_neighbor_count++; //increment count of live neighbors
                        }
                    }
                }
            }
        }
    }
    return live_neighbor_count; //return count of live neighbors
}

void generateNextStep(int* board, int* next_board, int boardRowSize, int boardColSize) { //function to generate next board (we will use this in updateBoard and aliveStable)
    int live_neighbor_count; //variable for number of live neighbors for each spot in board
    for (int r = 0; r < boardRowSize; r++) { //loop through every row in board
        for (int c = 0; c < boardColSize; c++) { //loop through every column in row
            live_neighbor_count = countLiveNeighbor(board, boardRowSize, boardColSize, r, c); //determine number of live neighbors for cell we are examining
            if (live_neighbor_count <= 1 || live_neighbor_count >= 4) { //if either of these conditions are met, cell will die on the next step
                next_board[r*boardColSize + c] = 0; //mark cell as dead for next game board
            } else if (!board[r*boardColSize + c] && live_neighbor_count == 3) {
                next_board[r*boardColSize + c] = 1; //if cell is dead and number of live neighbors is 3, it will come back to life
            } else {
                next_board[r*boardColSize + c] = board[r*boardColSize + c]; //if we aren't changing status of cell, keep it the same on the next board
            }
        }
    }
}

void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int next_board[boardRowSize*boardColSize]; //variable for next board
    generateNextStep(board, next_board, boardRowSize, boardColSize); //generate next board
    for (int r_next = 0; r_next < boardRowSize; r_next++) { //loop through every row in board
        for (int c_next = 0; c_next < boardColSize; c_next++) { //loop through every column in row
            board[r_next*boardColSize + c_next] = next_board[r_next*boardColSize + c_next]; //set game board values equal to next board's values
        }
    }
}

int aliveStable(int* board, int boardRowSize, int boardColSize){
    int next_board[boardRowSize*boardColSize]; //variable for next board
    generateNextStep(board, next_board, boardRowSize, boardColSize); //generate next board
    for (int r_next = 0; r_next < boardRowSize; r_next++) { //loop through every row in board
        for (int c_next = 0; c_next < boardColSize; c_next++) { //loop through every column in row
            if (board[r_next*boardColSize + c_next] != next_board[r_next*boardColSize + c_next]) {
                return 0; //if eny entries between board and next board aren't the same, configuration isn't stable
            }
        }
    }
    return 1; //if there are no differences between boards, stability has been reached
}