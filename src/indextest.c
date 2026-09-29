#include <stdio.h>
#include "indexing.h"

int main()
{
    WordNode *wordHead = NULL;

    insertWord(&wordHead, "data", "doc1.txt");
    insertWord(&wordHead, "data", "doc1.txt");
    insertWord(&wordHead, "machine", "doc1.txt");
    insertWord(&wordHead, "data", "doc2.txt");
    insertWord(&wordHead, "machine", "doc2.txt");

    displayIndex(wordHead);

    return 0;
}