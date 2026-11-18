#ifndef INDEXING_H
#define INDEXING_H

typedef struct PosNode{
    int line;
    long offset;
    int seq;
    struct PosNode *next;
} PosNode;

typedef struct DocNode{
    char filename[100];
    int frequency;
    PosNode *positions;
    PosNode *postail;
    struct DocNode *next;
} DocNode;

typedef struct WordNode{
    char word[100];
    DocNode *document;
    struct WordNode *next;
    struct WordNode *hashnext;
} WordNode;

//position node functions

PosNode *createPosNode(int line, long offset, int seq);

//Word functions

WordNode *createWordNode(const char *word);

void insertWord(WordNode **head,
                const char *word,
                const char *filename,
                int line, long offset, int seq);

extern WordNode *wordHead;

//document functions

DocNode *createDocNode(const char *filename);

void addDocument(WordNode *wordNode,
                 const char *filename,
                int line, long offset, int seq);

//utility function

void displayIndex(WordNode *head);

void searchWord(const char *word);
#endif