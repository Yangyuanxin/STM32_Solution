#include "Queue.h"

Status Queue_Init(Queue *q)
{
    q->front = q->rear = (QueuePtr) malloc(sizeof(QNode));
    if(!q->front) return -1 ;
    q->front->next = NULL;
    return OK;
}


Status Queue_Destroy(Queue *q)
{
    while(q->front)
    {
        q->rear = q->front->next;
        free(q->front);
        q->front = q->rear;
    }
    return OK;
}


Status Queue_Push(Queue *q, QElemType e)
{
    QueuePtr p = (QueuePtr) malloc(sizeof(QNode));
    if(!p) return -1 ;
    p->data = e;
    p->next = NULL;
    q->rear->next = p;
    q->rear = p;
    return OK;
}

Status Queue_Pop(Queue *q, QElemType *e)
{
    if(q->front == q->rear) return ERROR;
    QueuePtr p = (QueuePtr) malloc(sizeof(QNode));
    if(!p) return -1 ;
    p = q->front->next;
    *e = p->data;
    q->front->next = p->next;
    if(q->rear == p) q->rear = q->front;
    free(p);
    return OK;
}


int Test_Queue_Sample(void)
{
	Queue q;
	int e = 0 ;
	int i = 0 ;
	int rand_data = 0 ;
	Queue_Init(&q);
	//进队列
	printf("\n入队:\n");
    for (i = 0; i < 10; ++i){
		rand_data = rand()%(55000-46000)+46000; /*生成一个bai[t,d)区间内的du整数*/
        Queue_Push(&q, rand_data);
		printf("%d ",rand_data);
	}
	putchar('\n');
	printf("出队:\n");
    for (i = 0; i < 10; ++i){
        Queue_Pop(&q, &e);
		printf("%d ",e);
	}
	Queue_Destroy(&q);
	return OK ;
}

