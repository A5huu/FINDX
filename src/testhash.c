#include <stdio.h>
#include "text_extractor.h"
#include "hash.h"
#include "indexing.h"
#include <string.h>
int main()
{
    //WordNode *Hash_table[HASH_SIZE];
    //extractText("test1.txt");
    //return 0;
    initHashTable();
    WordNode testNode;
    strcpy(testNode.word, "machine");
    testNode.next = NULL;
    testNode.hashnext = NULL;
    testNode.hashnext = NULL;
    insertHash(&testNode);
    int index = hashFunction("machine");
    printf("'machine' -> Hash bucket %d", index);
    if(Hash_table[index] != NULL )
    {
        printf("\nHash table has the wordd -> %s\n", Hash_table[index]->word);
    }
    else
        printf("ts shit nt working bruv\n");

}
