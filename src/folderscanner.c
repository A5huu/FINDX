#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "folderscanner.h"
#include "text_extractor.h"

//queue node for directory traversal
void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
}

int isQueueEmpty(Queue *q)
{
    return q->front == NULL;
}

void enqueue(Queue *q, const char *path)
{
    QueueNode *new_node = (QueueNode *)malloc(sizeof(struct QueueNode));
    if(!new_node) return;
    strcpy(new_node->path, path);
    new_node->next = NULL;
    if(q->rear == NULL)
    {
        q->front = q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}

char* dequeue(Queue *q)
{
    if(isQueueEmpty(q)) return NULL;
    QueueNode *temp = q->front;
    static char path[512];
    strcpy(path, temp->path);
    q->front = q->front->next;
    if(q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);
    return path;
}

//queue based directory traversal
void scanDirectory(const char *root_path)
{
    Queue q;
    initQueue(&q);
    enqueue(&q, root_path);

    printf("[FINDX] Traversing directory '/%s'\n", root_path);
    while(!isQueueEmpty(&q))
    {
        char *current_path = dequeue(&q);
        DIR *dir = opendir(current_path);
        if(!dir) continue;

        struct dirent *entry;
        while((entry = readdir(dir)) != NULL)
        {

            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", current_path, entry->d_name);
            struct stat path_stat;
            if(stat(full_path, &path_stat) != 0) continue;

            if(S_ISDIR(path_stat.st_mode))
            {
                //addd subfolder to the traversal queue
                enqueue(&q, full_path);
            }
            else if(S_ISREG(path_stat.st_mode))
            {
                //check for .txt extension
                size_t len = strlen(full_path);
                if(len > 4 && strcmp(full_path + len-4,".txt") == 0)
                {
                    extractText(full_path);
                }
            } 
        }
        closedir(dir);
    }
}


