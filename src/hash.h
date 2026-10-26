#ifndef HASH_H
#define HASH_H

#include "indexing.h"

#define HASH_SIZE 100
extern WordNode *Hash_table[HASH_SIZE];
void initHashTable();
int hashFunction(const char *word);
void insertHash(WordNode *node);

#endif