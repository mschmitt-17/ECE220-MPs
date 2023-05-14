#include "sudoku.h"

/*
INTRO: for is_val_in_row, I simply iterated through every column in a given row and determined if the passed value was already in said row. Similarly,
for is_val_in_col, I iterated through every row in a given column and determined if the passed value was already in said column. For is_val_in_3x3zone,
I first had to determine which zone to check. I did this by declaring two variables, row_offset and col_offset, first filling row_offset with the
corresponding value for the correct set of rows to check, and then filling col_offset with the correct set of columns to check. I then checked every entry
in a 3x3 grid starting from the offsets to make sure the passed value wasn't already in the 3x3 zone. For is_val_valid, I declared a variable good_val to 
represent whether or not a value was valid for a given index in the sudoku, and set it equal to the return value of is_val_in_row. If it was still zero
after this I set it equal to the return value of is_val_in_col, and if it was still zero after this I set if equal to the return value of is_val_in_3x3zone.
If it was still zero after all of this the value is valid, and if not it is not valid. Accordingly, I return the negation of good_val. Finally, for solve_sudoku,
I essentially implemented the algorithm given on the wiki: check if the grid is full of nonzero values, if so return 1 (true), if not set and i and j equal to 
the row and column values of the zero value and check to see which numbers can be put in (i, j). If a given value does not lead to a solution (which we figure 
out by recursively calling solve_sudoku), change (i, j) back to 0 and try a different number.
*/

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  for (int column_index = 0; column_index < 9; column_index++) { //iterate through every column in row
    if (val == sudoku[i][column_index]) { //if we find val in row, return 1
      return 1;
    }
  }
  return 0;
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  for (int row_index = 0; row_index < 9; row_index++) { //iterate through every row in column
    if (val == sudoku[row_index][j]) { //if we find val in column, return 1
      return 1;
    }
  }

  return 0;
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  assert(j>=0 && j<9); //columns also need to be within indices 1 and 8
  int col_offset, row_offset; //declare variables to represent offset from 0 for rows and columns (will help us iterate through correct zone)

  if (i < 3) {
    row_offset = 0; //if i < 3, 3x3 zone will be starting at row 0
  } else if (i < 6) {
    row_offset = 3; //if 3 < i < 6, 3x3 zone will be starting at row 3
  } else {
    row_offset = 6; //if neither of the above conditions are met, 3x3 zone will be starting at row 6
  }
  if (j < 3) { //if j < 3, 3x3 zone will be starting at column 0
    col_offset = 0; 
  } else if (j < 6) { //if 3 < j < 6, 3x3 zone will be starting at column 3
    col_offset = 3;
  } else { //if neither of the above conditions are met, 3x3 zone will be starting at column 6
    col_offset = 6;
  }

  for (int column_index = 0; column_index < 3; column_index++) { //want to iterate through 3 rows
    for (int row_index = 0; row_index < 3; row_index++) { //want to iterate through 3 columns
      if (val == sudoku[row_offset + row_index][col_offset + column_index]) { //iterate through correct zone using predetermined offsets
        return 1;
      }
    }
  }
  
  return 0;
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);
  
  int good_val = 0; //declare variable to hold return value
  good_val = is_val_in_row(val, i, sudoku); //check if val is in row
  if (!good_val) {
    good_val = is_val_in_col(val, j, sudoku); //if val wasn't in row, check if it's in column
  }
  if (!good_val) {
    good_val = is_val_in_3x3_zone(val, i, j, sudoku); //if val wasn't in column, check if it's in 3x3 zone
  }
  
  return !good_val; //negation of good_val will be value we want to return after all of the above checks
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {
  int i, j;
  
  for (int sudoku_val = 0; sudoku_val < 81; sudoku_val++) { //loop through all values in sudoku grid
    int column = sudoku_val % 9; //column we are analyzing is element number % 9 (column numbers 0-8 in each row)
    int row = (sudoku_val - column) / 9; //row we are analyzing is the element number minus the column number divided by elements per row (9 elements)
    if (!sudoku[row][column]) { //if element is 0, set i and j to determined row and column values since the spot is empty and break out of loop
      i = row;
      j = column;
      break;
    }
    if (column == 8 && row == 8) { //if loop has come this far without breaking, all values are filled and sudoku is completed
      return true;
    }
  }

  for (int num = 1; num <= 9; num++) { //iterate through every potential number we can fill in sudoku with
    if (is_val_valid(num, i, j, sudoku)) {
      sudoku[i][j] = num; //try putting number into index (i, j)
      if (solve_sudoku(sudoku)) { //if this eventually solves the sudoku return true
        return true;
      }
      sudoku[i][j] = 0; //otherwise set index (i, j) back to 0, since num was not the right value to solve the puzzle
    }
  }
  return 0;
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





