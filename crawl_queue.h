#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/HTMLparser.h>

typedef struct Queue
{
    char *url;
    struct Queue *next;
} Node;
int MAX_SIZE = 500;
Node *front = NULL, *rear = NULL;

void create_Queue(char *url)
{
    Node *p = (Node *)malloc(sizeof(Node));
    p->url = url;
    p->next = NULL;
    front = rear = p;
    printf("%s\n", front->url);
    MAX_SIZE--;
}

int Queue_insert(char *url)
{
    if (MAX_SIZE == 0)
    {
        printf("Reached Max Queue size!!");
        return 0;
    }
    Node *p = (Node *)malloc(sizeof(Node));
    p->url = url;
    p->next = NULL;
    rear->next = p;
    rear = p;
    printf("%s\n", front->url);
    MAX_SIZE--;
    return 1;
}

void Queue_delete()
{
    if (!front && !rear)
    {
        printf("Queue is empty!!\n");
        return;
    }
    Node *temp = NULL;
    temp = front;
    front = front->next;
    free(temp);
}