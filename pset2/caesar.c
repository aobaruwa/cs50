#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>


//Convert an ascii character to an integer alphabet
int AsciiToAlpha(char ascii){
    int letter = 0;

    if (islower(ascii))
        letter = ascii - 'a';
    else
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
char encypher(char p, string key){
    char c;
    int d, num;
    num = atoi(key);
    if (isalpha(p)){
        d = AsciiToAlpha(p);
        d = (d + num) % 26;
        c = AlphaToAscii(d, p);

    return c;
    }
    else
        return p;

}
int main(int argc, string argv[])
{


    int i;

    void text(char buffer[]);
    int AsciiToAlpha(char ascii);
    char AlphaToAscii(int alpha, char leta);
    char encypher(char p, string key);

    //check for the correct number of command line arguments
    if (argc != 2){

        return 1;
    }


    printf("plaintext: ");

    string buffer = get_string();
    printf("ciphertext: ");
    for(i = 0; buffer[i] != '\0'; i++)
        printf("%c", encypher(buffer[i], argv[1]));

    printf("\n");
    return 0;
}
