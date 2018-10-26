#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main()
{
    int i, j, k, Height;

    /*Recieve input from user */
    do{
    printf("Height: ");
    Height = get_int();
    }
    /*chek if user inputs a nonnegative integer which is not greater than 23*/
    while (Height < 0 || Height > 23);




    /*nested for loops to implement the required number of hashes and spaces*/

    for(i = 1; i<=Height; i++)
    {
            for(j = 1;j<= Height - i;j++)
            {
                printf(" ");
            }
            for(k=1; k<= i+1;k++)
            {
                printf("#");
            }

            printf("\n");
    }

    return 0;
}
