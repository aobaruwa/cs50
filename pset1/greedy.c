#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cs50.h>

int main()
{
    int quarter = 0, dime = 0, nickel = 0, penny = 0, temp_quarter, temp_dime, temp_nickel, temp_penny, total;
    float amount;

    //prompts the user for his change
    printf("O hai! How much change is owed? \n");
    amount = get_float();

    //checking for correct input
    if(amount <= 0){
        printf("O hai! How much change is owed? \n");
        amount = get_float();
    }

    //taking care of the imprecision in float data types
    //by casting the amount into an integer
    amount = (int)round(amount *100);

    while(amount >= 25){
        temp_quarter = amount - 25;
         quarter++;
        amount = temp_quarter;

   }

    while (amount >= 10){
        temp_dime = amount - 10;
         dime++;
        amount = temp_dime;
    }


    while (amount >= 5){
        temp_nickel = amount - 5;
         nickel++;
        amount = temp_nickel;

    }
    while (amount >=1){
        temp_penny = amount - 1;
        penny++;
        amount = temp_penny;
    }



 //counts the number of coins
    total = quarter + dime + nickel + penny;
    printf("%d\n", total);


    return 0;
}
