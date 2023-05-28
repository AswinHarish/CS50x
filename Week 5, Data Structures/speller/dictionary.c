// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Other variables
unsigned int count;
unsigned int hashValue;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Checking if the word is in the dictionary or not
    hashValue = hash(word);
    node *current = table[hashValue];

    // Traversing through the linked list
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

// Hashes word to a number
unsigned int hash(const char *word)
{
    // hash function
    long total = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        total += tolower(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }

    // Adding to hash table
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }

        // adding the new node to linked list
        strcpy(new->word, word);
        hashValue = hash(word);
        new->next = table[hashValue];
        table[hashValue] = new;
        count++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Returns the size of words
    if (count > 0)
    {
        return count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // To free all malloc
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current)
        {
            // Creates temp to point to current
            node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    return true;
}
