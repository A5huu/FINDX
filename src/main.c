#include <stdio.h>
#include <string.h>
#include "searchengine.h"
#include "text_extractor.h"
#include "hash.h"
#include "indexing.h"

int main()
{
    //initialize the hash table
    initHashTable();

    printf("[FINDX] Initializing and indexing documents...\n");
    extractText("test1.txt");
    extractText("test2.txt");
    printf("[FINDX] Indexing complete");
    char query[200];

    while(1)
    {
        printf("\n==================================================\n");
        printf(" FINDX Local Search Engine (Type 'exit' to quit)\n");
        printf("==================================================\n");
        printf("Search query: ");
        if(fgets(query, sizeof(query), stdin) == NULL)
        {
            break;
        }

        query[strcspn(query, "\r\n")] = '\0';

        if(strcmp(query, "exit") == 0 || strcmp(query, "quit") == 0)
        {
            printf("\nExisting FINDX......");
            break;
        }

        //ignore empty inputs
        if(strlen(query) == 0)
        {
            continue;
        }

        //execute the full multiword search
        searchMultiWord(query);
    }
    return 0;
}
