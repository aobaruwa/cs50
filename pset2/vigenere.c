

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>


//Function that maps the alphabes in the plain text to their indexes
int enumerate(char buffer[], int pos){
    int i, counter = 0;
    for(i = 0; i <= pos; i++){
        if(isalpha(buffer[i]))
            counter++;

    }
    return counter - 1;



}
//Function that receives input and stores it as a text in an array named buffer
void text(char buffer[]){
    char character;
    int i = 0;

    do{
        character = getchar();
        buffer[i] = character;
        i++;
    }
    while(character != '\n');

    buffer[i - 1] = '\0';

}
//Convert an ascii character to an integer alphabet
int AsciiToAlpha(char ascii){
    int letter = 0;

    if (islower(ascii))
        letter = ascii - 'a';
    else if(isupper(ascii))
        letter = ascii - 'A';



    return letter;

}
//convert an integer alphabet back to an ascii character
char AlphaToAscii(int alpha, char leta){
    char letter = '\0';

    if (islower(leta))
        letter = alpha + 'a';
    else if (isupper(leta))
        letter = alpha + 'A';

    return letter;
}
char encypher(char p, int key){
    char c;
    int d;

        d = AsciiToAlpha(p);
        d = (d + key + 1) % 26;
        c = AlphaToAscii(d, p);

    return c;

}
//Output vigenere cypher character
char v_cypher(char buffer[], char buff, string keyword, int i){
    int j, key, num;
    char p;

    if (isalpha(buff)){
        num = enumerate(buffer, i);
        j = num  % strlen(keyword);
        key = AsciiToAlpha(keyword[j]);

        p = encypher(buff, key);
        return p;
        }
    else
        return buff;



}
int main(int argc, string argv[])
{

    int i = 0,j;
    char buffer[50];
    void text(char buffer[]);
    int enumerate(char buffer[], int pos);
    int AsciiToAlpha(char ascii);
    char AlphaToAscii(int alpha, char leta);
    char encypher(char p, int key);
    char v_cypher(char buffer[], char buff, string keyword, int i);

    if(argc !=2)
        return 1;
    for(j = 0; j < strlen(argv[1]); j++)
        if (!isalpha(argv[1][j]))
            return 1;

    printf("plaintext: ");
    text(buffer);
    printf("ciphertext: ");
   // printf("\n%c", encypher('a', 0));
    for(i = 0; buffer[i] != '\0'; i++)
        printf("%c", v_cypher(buffer, buffer[i], argv[1], i ));
    printf("\n");


    return 0;
}
