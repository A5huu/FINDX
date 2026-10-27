#include <stdio.h>
#include <string.h>
#include "indexing.h"
#include "hash.h"

WordNode *Hash_table[HASH_SIZE];

//function to initialize the hash table
void initHashTable()
{
    for(int i = 0; i < HASH_SIZE; i++)
    {
        Hash_table[i] = NULL;
    }
}

//function to calculate hash bucket
int hashFunction(const char *word)
{
    unsigned long hash = 0;
    for(int i = 0; word[i] != '\0'; i++)
    {
        hash = hash * 31 + word[i];
    }

    return hash % HASH_SIZE;
}

//
void insertHash(WordNode *node)
{
    int index = hashFunction(node->word);
    node->hashnext = Hash_table[index];
    Hash_table[index] = node;
}

//utility fucntion 
WordNode *lookUpHash(const char *word)
{
    int index = hashFunction(word);
    WordNode *current = Hash_table[index];
    while(current != NULL)
    {
        if(strcmp(word, current->word) == 0)
        {
            return current;
        }
        current = current->hashnext;
    }
    return NULL;
}
