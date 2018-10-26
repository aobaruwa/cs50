/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"
static int total = 0;

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
//declare 26 pointers to different linked lists
node *hashtable[26];

//Hash function
//Convert an ascii character to an integer alphabet
int hash(char ascii){
    int letter = 0;

    if (islower(ascii))
        letter = ascii - 'a';
    else if(isupper(ascii))
        letter = ascii - 'A';
    return letter;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
   int bucket = hash(word[0]);
   node *cursor = hashtable[bucket];
   while(cursor != NULL)
    {
        if(strcasecmp(word, cursor -> word) == 0)
            return true;
        else
            cursor = cursor ->next;
    }

    return false;
}



/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    int count[50], i;
    char Word[LENGTH + 1];

    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
        return false;

    for(int j = 0; j < 27; j++)
        count[j] = 0;

    while (fscanf(inptr, "%s", Word) != EOF)
    {
        node* new_node = calloc(1,sizeof(node));
        if(new_node == NULL)
        {

            unload();
            return false;
        }

        //fscanf(inptr, "%s", Word);
        strcpy (new_node->word, Word);
        total++;

        i = hash((new_node->word)[0]);
        if (count[i] == 0)
        {
            hashtable[i] = new_node;
            count[i]++;
        }
        else
        {
            new_node -> next = hashtable[i];
            hashtable[i] = new_node;
        }

    }
    hashtable[i +1] = NULL;
    fclose(inptr);
    return true;


}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return total;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{

    //int i;
    //node *cursor = NULL;
    for(int i = 0; i < 26; i++)
    {
      node *cursor = hashtable[i];
      while(cursor != NULL)
      {
      node *temp = cursor;
      cursor = cursor ->next;
      free(temp);
      }

   }

    return true;

}
