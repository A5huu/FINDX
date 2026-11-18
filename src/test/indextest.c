#include <stdio.h>
#include "indexing.h"
#include "hash.h"

int main()
{
    initHashTable();

    WordNode *wordHead = NULL;

// checking words indexing and hashing

    insertWord(&wordHead, "data", "doc1.txt");
    insertWord(&wordHead, "data", "doc1.txt");
    insertWord(&wordHead, "machine", "doc1.txt");
    insertWord(&wordHead, "data", "doc2.txt");
    insertWord(&wordHead, "machine", "doc2.txt");

    displayIndex(wordHead);

//verify hashtable by doing hash lookup
    WordNode *found = lookUpHash("machine");
    if(found != NULL)
    {
        printf("\nHash lookup was successful : %s\n", found->word);
    }
    else
    {
        printf("\nHash Lookup Failed!");
    }

    WordNode *collision1 = createWordNode("machine");
    WordNode *collision2 = createWordNode("data");

    printf("\nHash of machine: %d\n", hashFunction("machine"));
    printf("Hash of data: %d\n", hashFunction("data"));

    printf("\n--- Collision Test ---\n");

int foundCollision = 0;

for (int i = 0; i < 10000 && !foundCollision; i++)
{
    for (int j = i + 1; j < 10000; j++)
    {
        char word1[20];
        char word2[20];

        sprintf(word1, "word%d", i);
        sprintf(word2, "word%d", j);

        if (hashFunction(word1) == hashFunction(word2))
        {
            printf("Collision pair: %s and %s\n", word1, word2);
            printf("Bucket: %d\n", hashFunction(word1));

            foundCollision = 1;
            break;
        }
    }
}

printf("\n--- Collision Handling Test ---\n");

insertWord(&wordHead, "word0", "doc1.txt");
insertWord(&wordHead, "word66", "doc2.txt");

WordNode *found0 = lookUpHash("word0");
WordNode *found66 = lookUpHash("word66");

if (found0 != NULL && found66 != NULL)
{
    printf("Both collision words found successfully!\n");
    printf("%s -> bucket %d\n",
           found0->word, hashFunction(found0->word));
    printf("%s -> bucket %d\n",
           found66->word, hashFunction(found66->word));
}
else
{
    printf("Collision handling failed!\n");
}

    return 0;

    
}