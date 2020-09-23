#ifndef __QUEUE_H
#define __QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define NOPE 0
#define ERROR 0
#define OVERFLOW -2

typedef int QElemType; 

typedef struct qNode
{
    QElemType data;
    struct qNode *next;
} QNode, *QueuePtr;

typedef int Status; 

typedef struct
{
   QueuePtr front;
   QueuePtr rear;
} Queue;


Status Queue_Init(Queue *q);
Status Queue_Destroy(Queue *q);
Status Queue_Push(Queue *q, QElemType e);
Status Queue_Pop(Queue *q, QElemType *e);
int Test_Queue_Sample(void);

#endif //__QUEUE_H
