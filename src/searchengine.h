#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

int tokenizeQuery(const char *query, char tokens[][100], int max_token_size);

int filterTokens(char tokens[][100], char filtered[][100],
                 int token_count, int max_filtered_size);

#endif