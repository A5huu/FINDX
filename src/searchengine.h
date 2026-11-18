#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "indexing.h"

//Helper structure to track document match candidates during search
typedef struct SearchCandidate{
    char filename[100];
    int matched_terms_count;
    int total_frequency;
    int exact_match;
} SearchCandidate;

typedef struct HistoryNode
{
    char query[200];
    struct HistoryNode *next;
} HistoryNode;

void initHistoryStack();
void pushHistory(const char *query);
void displayHistory();

//tokenize text query
int tokenizeQuery(const char *query, char tokens[][100], int max_token_size);

//filter out filler worfs from tokens
int filterTokens(char tokens[][100], char filtered[][100],
                 int token_count, int max_filtered_size);

//perform multiword search with teirs
void searchMultiWord(const char *query);

#endif