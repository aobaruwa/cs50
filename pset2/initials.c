#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>


//Function that extracts the initials of the array buffer
void initial(char buffer[]){
    bool isLookingForWord = true;

    int j = 0, i;
    char initials[20];

    for (i = 0; buffer[i] != '\0'; i++){
        if(isalpha(buffer[i]))
            {
           if (isLookingForWord)
           {
            initials[j] = buffer[i];
            isLookingForWord = false;
            j++;
           }
        }
       else
        isLookingForWord = true;
    }
    initials[j] = '\0';
   for(j = 0; j < strlen(initials); j++)
    printf("%c", toupper(initials[j]));
    printf("\n");
}
int main()
{
    void store(char buffer[]);
    void initial(char buffer[]);

    string buffer = get_string();
    initial(buffer);




    return 0;
}
