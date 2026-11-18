#ifndef FOLDERSCANNER_H
#define FOLDERSCANNER_H

//queue node for directory traversal
typedef struct QueueNode
{
    char path[512];
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

void scanDirectory(const char *base_path);

#endif