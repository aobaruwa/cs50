#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
int main()
  {
    int bottles;

    printf("minutes: \n");

    int minutes = get_int();

    bottles = (12 * minutes);

    printf("bottles: %d  \n", bottles);
   }
