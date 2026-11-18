#include <stdio.h>
#include "searchengine.h"

static void run(const char *q) {
    char tokens[20][100], filtered[20][100];
    int n = tokenizeQuery(q, tokens, 20);
    int m = (n > 0) ? filterTokens(tokens, filtered, n, 20) : n;
    printf("query: \"%s\"\n  tokens(%d):", q, n);
    for (int i = 0; i < n; i++) printf(" [%s]", tokens[i]);
    printf("\n  filtered(%d):", m);
    for (int i = 0; i < m; i++) printf(" [%s]", filtered[i]);
    printf("\n\n");
}

int main(void) {
    run("machine learning is useful");
    run("Machine LEARNING");
    run("the is a");
    run("");
    return 0;
}