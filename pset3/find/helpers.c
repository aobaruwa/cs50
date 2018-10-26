/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
   int start, end, middle;
   start = 0;
   end = n;
   while (start <= end){

       middle = (start + end)/2;
       if (values[middle] == value)
        return true;
       else if (value > values[middle]){
        start = middle + 1;
        end = n -1;
       }
       else if (value < values[middle]){
        end = middle - 1;
       }


   }


    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int swapCounter = -1;
    int j = 0, temp = 0;

while (swapCounter != 0){
    swapCounter = 0;
    for(j = 0; j < n - 1; j++){
       if(values[j] > values[j + 1]){
            temp = values[j];
            values[j] = values[j + 1];
            values[j + 1] = temp;
            swapCounter++;
            }

        }


}









    // TODO: let's implement the bubble sorting algorithm
}

//test main
/*int main(void){

    void sort(int values[], int n);
    bool search(int value, int values[], int n);

    int values[] = {11, 23, 8, 14, 30, 9, 6, 17, 22, 28, 25, 15, 7, 10, 19};
    sort(values, 15);

        printf("%i\n", search(19, values, 15));

return 0;
}*/
