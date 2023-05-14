#include <stdlib.h>
#include <stdio.h>
/*
First Error: missing semicolon in main.c, causing the program to fail to compile
             (fixed by adding semicolon)
Second Error: return values reversed in is_prime, causing the function to return 
              0 when a prime was found and 1 when a non-prime value was found
              (fixed by reversing the aforementioned return values)
Third Error: k was set equal to i%j instead of i/j, causing it to always be 0 due
             to the condition i%j == 0 required previously
             (fixed by replacing the aforementioned modulus operator to a division operator)
Fourth Error: 0 considered as a prime number, causing k values of 0 to erroneously
              be considered prime
              (fixed by adding an extra condition in the first if statement of is_prime)
Fifth Error: Once a number is found to be semiprime, iteration through j was continuing,
             meaning values could be printed multiple times for each prime number in their
             respective semiprime expression (e.g. 6 was printed once for j=2, k=3 and again
             for j=3, k=2) 
             (fixed by adding a break statement after the print statement in print_semiprimes)
*/

/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number == 1 || number == 0) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <=b; i++) { //for each item in interval
        //check if semiprime
        for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
                    k = i/j;
                    if (is_prime(k)) {
                        printf("%d ", i);
                        ret = 1;
                        break;
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;

}
