#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

/*
INTRO: I'll start by going over the 2 helper methods at the beginning: insert_tuple and delete_tuple. insert_tuple is a recursive function that walks the list and (as its name would suggest)
either inserts the tuple into the correct spot in the list based on the coordinates, or changes the value at said coordinates if there is a tuple for it already. It adjusts the pointers for
any tuples involved using the process discussed in lecture. delete_tuple does the opposite of insert_tuple, removing and freeing the tuple at a particular set of coordinates if it exists. I
used this helper whenever a matrix value would be set to 0, as for the sparse matrix we don't consider zero values. For load_tuples, I simply opened the provided file, scanned it for parameters
and initialized the sp_tuples pointer accordingly, and then iterated through the file using a while loop and feof, scanning row, column, and float parameters and calling insert_tuple and
delete_tuple to populate the linked list. Note insert_tuple and delete_tuple return integers to allow us to populate the nz field of the sp_tuples struct. For gv_tuples, I used a helper method
called get_tuple to recursively iterate through the passed linked list looking for a sp_tuples_node with a matching row and col, returning a pointer to the node or NULL if the node with the
corresponding row and col wasn't found. gv_tuples will then return 0 if the return value of get_tuple is null, or the value of the returned pointer otherwise. Set tuple simply uses insert_tuple
and delete_tuple, updating the nz of the sp_tuples struct accordingly. save_tuples first prints matrix dimensions to the passed output file, before calling helper function save_helper to walk
the linked list of the sp_tuples struct passed to save_tuples to print the row, column, and value of every entry to the output file. Add tuples first checks if the dimensions of the passed matrices
match, before allocating memory and properly initializing a new matrix, and calling helper method add_tuples_helper to recursively iterate through every entry in the non-destination matrix, adding
the entries at the corresponding rows and columns of the destination matrix with those found by walking the entries in the non-destination matrix. This process is conducted for both matrices passed
to add_tuples, following the algorithm on the wiki. destroy_tuples uses delete_tuple to iterate through the sp_tuples struct, using the head pointer and deleting nz-1 entries. After this the head
pointer itself is freed, corresponding to the last unfreed sp_tuples_node in the sp_tuples struct, and the sp_tuples struct itself is freed.
*/

int insert_tuple(sp_tuples_node **list, int row, int col, double value) { //helper function to add tuple to list in load_tuples; returns 1 if successful, 0 if there was already a nonzero entry at coords.
    sp_tuples_node *current = *list; //pointer to current tuple we are checking
    sp_tuples_node *temp; //temp pointer for if we need to add a tuple to the list
    if (current == NULL || row < current->row || (row == current->row && col < current->col)) { //base case, insert new node at head
        temp = (sp_tuples_node *)malloc(sizeof(sp_tuples_node)); //allocate memory for new tuple in temp
        temp->row = row; //set row of new tuple to passed row
        temp->col = col; //set column of new tuple to passed column
        temp->value = value; //set value of new tuple to passed value
        temp->next = current; //set next pointer of temp to current node
        *list = temp; //update list head pointer
        return 1;
    }
    if ((*list)->row == row && (*list)->col == col) { //base case, coordinate has already been placed in matrix
        current->value = value;
        return 0;
    }
    return insert_tuple(&(current->next), row, col, value); //recursive case
}

int delete_tuple(sp_tuples_node **list, int row, int col) { //helper function to use in case value of tuple is changed to 0; returns -1 if successful, 0 if there was no nonzero entry to delete
    sp_tuples_node *current = *list; //pointer to current tuple we are checking
    if (current == NULL || row < current->row || (row == current->row && col < current->col)) { //base case, if end of list is reached or coordinate is exceeded
        return 0;
    }
    if (current->row == row && current->col == col) { //base case, if we find an entry that matches passed row and column
        *list = current->next; //set next pointer of previous entry to next entry in list
        free(current); //free current tuple
        return -1;
    }
    return delete_tuple(&(current->next), row, col); //recursive case
}

sp_tuples_node *get_tuple(sp_tuples_node *current, int row, int col) { //helper function to return pointer to tuple if it is in list
    if (current == NULL || row < current->row || (row == current->row && col < current->col)) { //base case, if coordinate is exceeded
        return NULL;
    }
    if (current->row == row && current->col == col) { //base case, if coordinate is found
        return current;
    }
    return get_tuple(current->next, row, col); //recursive case, if current tuple isn't the one we're looking for or past the one we're looking for
}

sp_tuples * load_tuples(char* input_file)
{
    sp_tuples *tuple_list = (sp_tuples *)malloc(sizeof(sp_tuples)); //allocate memory for our list of tuples 
    FILE* in_file = fopen(input_file, "r"); //open file in read mode
    int row, col; //declare variables for row and column of matrix
    fscanf(in_file, "%d %d", &row, &col); //scan dimensions of matrix into declared variables
    tuple_list->m = row; //add proper row dimension into our tuple list
    tuple_list->n = col; //add proper column dimension into our tuple list
    tuple_list->tuples_head = NULL;
    tuple_list->nz = 0; //initialize nonzero entries as zero
    while (!feof(in_file)) { //while we still have entries to read
        int nz_update = 0;
        int new_row, new_col; //declare variables for row, column of new tuple
        double new_value; //decalre variable for value of new tuple
        fscanf(in_file, "%d %d %lf", &new_row, &new_col, &new_value); //scan values into newly declared variables
        if (new_value == 0) { //if value is 0, it is equivalent to deleting the entry from the list (sparse matrix)
            nz_update = delete_tuple(&(tuple_list->tuples_head), new_row, new_col);
        } else { //if value is nonzero, we must add it to the list
            nz_update = insert_tuple(&(tuple_list->tuples_head), new_row, new_col, new_value);
        }
        tuple_list->nz += nz_update; //update number of nonzero entries
    }
    fclose(in_file);
    return tuple_list;
}



double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    sp_tuples_node *current = mat_t->tuples_head; //variable for head of tuple list
    sp_tuples_node *return_node = get_tuple(current, row, col); //use helper function to recurse through list, get pointer to tuple
    if (return_node == NULL) { //if get_tuple returns null, value at coordinate is zero
        return 0;
    }
    return return_node->value; //otherwise, the tuple has a nonzero value and we return it
}



void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
    int nz_update = 0; //variable to update nz attribute of mat_t
    if (value == 0) { //if we want to set the value to 0, it's the same as deleting it from the list. Accordingly, we use our previous helper function
        nz_update = delete_tuple(&(mat_t->tuples_head), row, col);
    }
    else { //if we want to set the value to a nonzero value, it's the same as adding it to the list. Accordingly, we use our previous function
        nz_update = insert_tuple(&(mat_t->tuples_head), row, col, value);
    }
    mat_t->nz += nz_update; //update nz based on return value of helper function called
    return;
}

void save_helper(FILE *write_file, sp_tuples_node *current) { //recursive function to walk list for save
    if (current==NULL) { //base case
        return;
    }
    fprintf(write_file, "%d %d %f\n", current->row, current->col, current->value); //scan desired values into passed file
    return save_helper(write_file, current->next); //do the same for the next file
}

void save_tuples(char * file_name, sp_tuples * mat_t)
{
    FILE *out_file = fopen(file_name, "w"); //open file in write mode
    fprintf(out_file, "%d %d\n", mat_t->m, mat_t->n); //write row, column value to output file
    save_helper(out_file, mat_t->tuples_head); //call helper function to walk list
    fclose(out_file); //close file after writing to it
	return;
}

void add_tuples_helper(sp_tuples_node *summand_pointer, sp_tuples *destination, sp_tuples *summand) { //helper method for add_tuples to add individual tuples
    if (summand_pointer == NULL) { //base case, if we've reached end of list return
        return;
    }
    double add_val_dest, add_val_new; //variables for terms we will add
    int row, col; //variables for row and column of entries we want to add
    row = summand_pointer->row; //assign row value based on nonzero entry in summand
    col = summand_pointer->col; //assign col value based on nonzero entry in summand
    add_val_dest = gv_tuples(destination, row, col); //use gv_tuples to retrieve value at given coordinate
    add_val_new = gv_tuples(summand, row, col); //use gv_tuples to retrieve value at given coordinate
    destination->nz += insert_tuple(&(destination->tuples_head), row, col, add_val_new + add_val_dest); //insert sum at same row and column in destination matrix, change nz of destination accordingly
    return add_tuples_helper(summand_pointer->next, destination, summand); //call function again to continue walking list
}

sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
    if (matA->m != matB->m || matA->n != matB->n) { //if dimensions of matrices don't match, addition isn't possible
        return NULL;
    }
    sp_tuples *retmat = (sp_tuples *)malloc(sizeof(sp_tuples)); //allocate memory for our matrix we will return
    retmat->m = matA->m; //set dimensions of new matrix
    retmat->n = matA->n;
    retmat->nz = 0; //initialize number of nonzero entries as 0
    retmat->tuples_head = NULL;
    sp_tuples_node *current = matA->tuples_head;
    add_tuples_helper(current, retmat, matA); //use helper function to walk matA and add all of our entries
    current = matB->tuples_head; //same process as above, but for matB
    add_tuples_helper(current, retmat, matB);
	return retmat;
}

/*

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    return retmat;

}
*/

	
void destroy_tuples(sp_tuples * mat_t) {
    for (int i = 0; i < mat_t->nz - 1; i++) { //iterate through every nonzero entry in matrix
        delete_tuple(&(mat_t->tuples_head), mat_t->tuples_head->row, mat_t->tuples_head->col); //delete_tuple will change where the head pointer points, so we delete every tuple except for one
    }
    free(mat_t->tuples_head); //delete last tuple (if we delete all in previous step this will free memory location 0)
    free(mat_t); //free the matrix struct itself
    return;
}  
