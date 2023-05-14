//partners: jrvasko2, asr9
/* intro: for this program, we first get the user's row input. After this, we print 1, since
row will always be >= 0 so each row of the triangle consists of at least a 1. After this, we 
print coefficients following the given formula until we have printed #row coefficients (row + 1) 
with initial 1.
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;

  printf("Enter a row index: ");
  scanf("%d",&row); //get user row input
  printf("%d", 1);  //row will always be >= 0, we always want to print a 1
  unsigned long to_print = 1; //initialize variable to print our values
  for (int i = 1; i < row + 1; i++) { //start at row i=1 since "1" printed before accounts for i=0 
                                      //# of coefficients to print = row + 1
    to_print *= (row + 1 - i); //multiply to_print by (n + 1 - i)
    to_print /= i; //divide by i
    printf(" %lu", to_print); //print to_print
  }
  return 0;
}
