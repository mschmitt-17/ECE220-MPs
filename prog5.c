/*			
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/* Intro: for set_seed, I used sscanf to determine if the seed input was valid, returning 1 if valid and printing an error message and 
 returning 0 if not. For start_game, I declared a variable to use to assign random values to our solutions, using rand(), the modulus
 operator, and adding 1 to the variable to ensure the random values were between 1 and 8. I then assigned guess_number to 1 to indicate
 that the first guess is about to be made. For make_guess, I declared variables to assign the digits of the guess to, declared 
 an array to manage my pairings, declared variables for any part of the guess after the 4 integers, and declared variables for the 
 number of perfect and misplaced matches. I then used sscanf to make sure exactly 4 integers were typed in for the guess. After this I check 
 that each integer is in the correct range, before assigning the guesses to the locations in memory meant to store the guesses. Finally, I 
 checked for perfect matches, checked for misplaced matches, printed the number of each, and incremented the guess number counter.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;

int
set_seed (const char seed_str[])
{   
    int seed;       //declare variable for seed value
    char post[2];       //declare variable for anything after initial integer value
    int scan_return = sscanf (seed_str, "%d%1s", &seed, post);
    if (scan_return == 1) {
        srand(seed);        //if return value for scanf is 1, only a single integer value was detected, so seed input was correct
        return 1;
    } else {
        printf("set_seed: invalid seed\n");     //if the return value for sscanf isn't 1, there either wasn't anything entered or more than a single integer value
        return 0;
    }
}

void
start_game (int* one, int* two, int* three, int* four)
{
    int assign_val;     //assign_val is variable we will use to assign values to our solutions (I didn't want to customize the loop for each solution digit)
    assign_val = rand();        //assign_val is now a pseudo-random number
    assign_val = assign_val % 8;        //assign_val is now some random number between 0 and 7
    assign_val = assign_val + 1; //assign_val is now some random number between 1 and 8
    *one = assign_val;      //write assign_val to memory location "one" points to
    solution1 = assign_val;     //set static variable for solution digit to assign_val
    assign_val = rand();
    assign_val = assign_val % 8;
    assign_val = assign_val + 1;
    *two = assign_val;      //write assign_val to memory location "two" points to
    solution2 = assign_val;
    assign_val = rand();
    assign_val = assign_val % 8;
    assign_val = assign_val + 1;
    *three = assign_val;      //write assign_val to memory location "three" points to
    solution3 = assign_val;
    assign_val = rand();
    assign_val = assign_val % 8;
    assign_val = assign_val + 1;
    *four = assign_val;      //write assign_val to memory location "four" points to
    solution4 = assign_val;
    guess_number = 1;
    
}

int
make_guess (const char guess_str[], int* one, int* two, int* three, int* four)
{
    int w, x, y, z;     //variables to write digits of guess_str to
    char paired[4];     //declare array to manage our different pairings
    paired[0] = 0;      //
    paired[1] = 0;      //-----> set each element of paired array to 0 to indicate no pairings
    paired[2] = 0;      //
    paired[3] = 0;      //
    char post[2];   //variable to store arguments after digits if there are any
    int perfect_matches = 0, misplaced_matches = 0;     //variables for number of perfect and misplaced matches
    int scan_return = sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post);
    if (scan_return == 4) {     //if exactly 4 arguments returned, all 4 digits have been filled and nothing more
        if (w > 8 || w < 1 || x > 8 || x < 1 || y > 8 || y < 1 || z > 8 || z < 1) {     //if guess numbers are outside 1-8, guess is invalid
            printf("make_guess: invalid guess\n");
            return 0;
        }
        *one = w;   //------>assign guess locations in memory to guess values
        *two = x;   //
        *three = y; //
        *four = z;  //
        //check for perfect matches
        if (w == solution1) {
            perfect_matches++;  //increment perfect_matches
            paired[0] = 'w';    //set value in paired to match corresponding variable 
        }
        if (x == solution2) {
            perfect_matches++;  //increment perfect_matches
            paired[1] = 'x';    //set value in paired to match corresponding variable 
        }
        if (y == solution3) {
            perfect_matches++;  //increment perfect_matches
            paired[2] = 'y';    //set value in paired to match corresponding variable 
        }
        if (z == solution4) {
            perfect_matches++;  //increment perfect_matches
            paired[3] = 'z';    //set value in paired to match corresponding variable 
        }
        //check for misplaced matches
        if (paired[0] != 'w') { //if w isn't perfect matched, we check for misplaced matches
            for (int i = 0; i < 1; i++) { //only want to run through the following loop a single time
                if (!paired[1]) {   //if solution2 isn't paired yet, we check for a misplaced match with w
                    if (w == solution2) {
                        misplaced_matches++;
                        paired[1] = 'w';
                        break; //if w is paired, we don't want to proceed with loop, as this might result in overcounting misplaced matches
                    }
                } if (!paired[2]) {   //if solution3 isn't paired yet, we check for a misplaced match with w
                    if (w == solution3) {
                        misplaced_matches++;
                        paired[2] = 'w';
                        break;
                    }
                } if (!paired[3]) { //if solution4 isn't paired yet, we check for a misplaced match with w
                    if (w == solution4) {   
                        misplaced_matches++;
                        paired[3] = 'w';
                        break;
                    }
                }
            }
        }
        if (paired[1] != 'x') { //if x isn't perfect matched, we check for misplaced matches
            for (int i = 0; i < 1; i++) { //only want to run through the following loop a single time
                if (!paired[0]) {      //if solution1 isn't paired yet, we check for a misplaced match with x
                    if (x == solution1) {
                        misplaced_matches++;
                        paired[0] = 'x';
                        break;
                    }
                } if (!paired[2]) {   //if solution3 isn't paired yet, we check for a misplaced match with x
                    if (x == solution3) {
                        misplaced_matches++;
                        paired[2] = 'x';
                        break;
                    }
                } if (!paired[3]) {   //if solution4 isn't paired yet, we check for a misplaced match with x
                    if (x == solution4) {
                        misplaced_matches++;
                        paired[3] = 'x';
                        break;
                    }
                }
            }
        }
        if (paired[2] != 'y') { //if y isn't perfect matched, we check for misplaced matches
            for (int i = 0; i < 1; i++) { //only want to run through the following loop a single time
                if (!paired[0]) {   //if solution1 isn't paired yet, we check for a misplaced match with y
                    if (y == solution1) {
                        misplaced_matches++;
                        paired[0] = 'y';
                        break;
                    }
                } if (!paired[1]) {   //if solution2 isn't paired yet, we check for a misplaced match with y
                    if (y == solution2) {
                        misplaced_matches++;
                        paired[1] = 'y';
                        break;
                    }
                } if (!paired[3]) {   //if solution4 isn't paired yet, we check for a misplaced match with y
                    if (y == solution4) {
                        misplaced_matches++;
                        paired[3] = 'y';
                        break;
                    }
                }
            }
        }
        if (paired[3] != 'z') { //if z isn't perfect matched, we check for misplaced matches
            for (int i = 0; i < 1; i++) { //only want to run through the following loop a single time
                if (!paired[0]) {      //if solution1 isn't paired yet, we check for a misplaced match with z
                    if (z == solution1) {
                        misplaced_matches++;
                        paired[0] = 'z';
                        break;
                    }
                } if (!paired[1]) {   //if solution2 isn't paired yet, we check for a misplaced match with z
                    if (z == solution2) {
                        misplaced_matches++;
                        paired[1] = 'z';
                        break;
                    }
                } if (!paired[2]) {   //if solution3 isn't paired yet, we check for a misplaced match with z
                    if (z == solution3) {
                        misplaced_matches++;
                        paired[2] = 'z';
                        break;
                    }
                }
            }
        }
        printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, perfect_matches, misplaced_matches);
        guess_number++;     //increment guess_number
    } else {    //if less or more than 4 arguments returned, guess was invalid
        printf("make_guess: invalid guess\n");
        return 0;
    }
    return 1;
}


