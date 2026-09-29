#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexing.h"

DocNode *createDocNode(const char *filename)
{
    DocNode *new_node = (DocNode *)malloc(sizeof(DocNode));
    
    if(new_node == NULL)
    {
        printf("Memory Allocation Failed");
        return NULL;
    }
    strcpy(new_node->filename, filename);
    new_node->frequency = 1;
    new_node->next = NULL;
    return new_node;
}

WordNode *createWordNode(const char *word)
{
    WordNode *new_node = (WordNode *)malloc(sizeof(WordNode));
    if(new_node == NULL)
    {
        printf("Memory Allocation Failed!");
        return NULL;
    }
    strcpy(new_node->word, word);
    new_node->document = NULL;
    new_node->next = NULL;
    return new_node;
}

//add document to the document list
void addDocument(WordNode *new_word_node,
                 const char *filename)
{
    DocNode *temp = new_word_node->document;
    //check if document already exists in list
    while(temp != NULL)
    {
        if(strcmp(temp->filename, filename) == 0)
        {
            temp->frequency++;
            return;
        }
        temp = temp->next;
    }
    //create if document does not exists in list
    DocNode *new_document = createDocNode(filename);
    if(new_document == NULL)
    {
        return;
    }
    
    new_document->next   = new_word_node->document;
    new_word_node->document = new_document;
}
void insertWord(WordNode **head,
                const char *word,
                const char *filename)
{
    WordNode *temp = *head;
    //check if word already exists
    while(temp != NULL)
    {
        if(strcmp(temp->word, word) == 0)
        {
            //update if it already exist
            addDocument(temp ,filename);
            return;
        }
        temp = temp->next;
    }
    //word does not exist so cerate new
    WordNode *new_word = createWordNode(word);
    if(new_word == NULL)
        return;
    
    //add document containing the word to the list
    addDocument(new_word, filename);
    new_word->next = *head;
    *head = new_word;
}

//utility function to verify
void displayIndex(WordNode *head)
{
    WordNode *word_current = head;

    while(word_current != NULL)
    {
        printf("\nword : %s\n", word_current->word);
        DocNode *doc_current = word_current->document;

        while(doc_current != NULL)
        {
            printf("Document : %s | Frequency : %d\n", doc_current->filename, doc_current->frequency);
            doc_current = doc_current->next;
        }
        word_current = word_current->next;
    }
}