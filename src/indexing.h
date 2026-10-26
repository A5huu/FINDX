#ifndef INDEXING_H
#define INDEXING_H

typedef struct DocNode{
    char filename[100];
    int frequency;
    struct DocNode *next;
} DocNode;

typedef struct WordNode{
    char word[100];
    DocNode *document;
    struct WordNode *next;
    struct WordNode *hashnext;
} WordNode;

//Word functions

WordNode *createWordNode(const char *word);

void insertWord(WordNode **head,
                const char *word,
                const char *filename);

//document functions

DocNode *createDocNode(const char *filename);

void addDocument(WordNode *wordNode,
                 const char *filename);

//utility function

void displayIndex(WordNode *head);
#endif