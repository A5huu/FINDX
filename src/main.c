#include "text_extractor.h"
#include "hash.h"
#include "indexing.h"

int main()
{
    initHashTable();
    extractText("test1.txt");
    extractText("test2.txt");
    displayIndex(wordHead);
    return 0;
}
