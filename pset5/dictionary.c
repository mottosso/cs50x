/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "dictionary.h"

/**
 * Global variables
*/
static Node** table;

/**
 * Larson's hash
 *
 * This hash function was chosen for it's low collision
 * rate amongst short keys, as opposed to functions by
 * Paul Hsieh and Bob Jenkins which are tunes for
 * long keys.
 *
 * Modifications:
 *   - Variable length replaced with fixed LENGTH
 *   - Resulting hash truncated to TABLESIZE
 *   - Loop broken at the initial discovery of \0
 *   - Only consider lowercase characters
 *
 * Original: http://www.strchr.com/hash_functions
 */
unsigned int hash(const char *key)
{
    unsigned int h = 0;
    for(unsigned int i = 0; i < LENGTH; ++i)
    {
        // Optimisation
        //
        // Due to LENGTH of words being of fixed size,
        // a majority of each word consists \0
        if (key[i] == '\0')
        {
            break;
        }

        h = 101 * h + tolower(key[i]);
    }
    return (h ^ (h >> 16)) % TABLESIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    unsigned int index = hash(word);

    // does it exist in our table at all?
    if (table[index] == NULL)
    {
        return false;
    }

    // does it exist in that particular linked list?
    Node* current = table[index];
    while (current != NULL)
    {
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Allocate memory for table
    //
    // At ~34.000.000 slots and ~140.000 dictionary words,
    // collisions happen ~300 times.
    table = (Node **) calloc(TABLESIZE, sizeof(Node*));

    if (table == NULL)
    {
        printf("Could not allocate enough memory for table.\n");
        return false;
    }

    FILE* fp = fopen(dictionary, "rb");

    if (fp == NULL)
    {
        return false;
    }

    Node* node = (Node *) calloc(1, sizeof(Node));
    while(fscanf(fp, "%s", node->word) != EOF)
    {        
        unsigned int index = hash(node->word);

        // prepend to existing node; highest first
        if (table[index] != NULL)
        {
            node->next = table[index];
        }

        table[index] = node;

        node = (Node *) calloc(1, sizeof(Node));
    }

    // When while finishes, one node is left unused
    free(node);

    fclose(fp);

    return true;
}

/**
 * Returns number of root in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    unsigned int count = 0;
    for (int i = 0; i < TABLESIZE; i++)
    {
        if (table[i] != NULL)
        {
            // Account for children
            Node* current = table[i];
            while (current != NULL)
            {
                count++;
                current = current->next;
            }
        }
    }
    
    return count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < TABLESIZE; i++)
    {
        if (table[i] == NULL)
            continue;

        Node* current = table[i];
        while(current != NULL)
        {
            Node* tmp = current->next;
            free(current);
            current = tmp;
        }
    }

    free(table);
    
    return true;
}
