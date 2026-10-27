#ifndef HASH_H
#define HASH_H

#include "indexing.h"

#define HASH_SIZE 100
extern WordNode *Hash_table[HASH_SIZE];

//hash initialization fucntion
void initHashTable();

//fucntion to calculate hash bucket
int hashFunction(const char *word);


void insertHash(WordNode *node);

//utility function for searching hash
WordNode *lookUpHash(const char *word);
#endif