#include "slist.h"

//创建节点
Node* makeNode(const int data)
{
    Node * p = (Node *)malloc(sizeof(Node));
    assert(p != NULL);
    p->data = data;
    p->next = NULL;
    return p;
}

//销毁节点
void destroyNode(Node* node)
{
    free((Node *) node);
}

//带头节点的单链表
void sInit(Slist * list)
{
    list->head = makeNode(INI_MIN);
}

//按顺序插入
bool sInsert(Slist * list, const int data)
{
    Node * ptem = list->head;
    Node* current;
    Node * node;
    node = makeNode(data);
    if(ptem->data > data)
    {
        list->head = node;
        node->next = ptem;
    } else {
        while((current = ptem->next) != NULL && current->data < data )
            ptem = ptem->next;
        ptem->next = node;
    }
    return true;
}

//移除节点
bool sRemove(Slist * list, const int key)
{
    Node* previous = list->head;
    Node* current;
    //找到删除节点
    while( ( current = previous->next ) != NULL && current->data != key)
        previous = previous->next;
    if ( current == NULL )
        return false;
    previous->next = current->next;
    free( current );
    return true;
}

//修改,先删后插入，因为这是有序链表
bool sModify(Slist * list, const int key, const int data)
{
    if( sRemove(list, key) )
        sInsert(list, data);
    else
        return false;
    return true;
}

//找到返回关键字的节点，否则返回null指针
Node* sFind(Slist * list, const int key)
{
    Node * current = list->head;
    while ( (current = current->next) != NULL && current->data != key)
        if( current->data > key )
            return NULL;
    return current;
}

//遍历
void sTreaverse( Slist * slist, void (*func) (Node* p) )
{
    Node * current = slist->head;
    func(current);
    while ( (current = current->next) != NULL )
        func(current);
}

//销毁节点
void sDestrory(Slist * list)
{
    sTreaverse(list, destroyNode);
    free(list->head);
}

//print
void printData(Node * p)
{
    printf("%d ", p->data);
}

void slist_sample(void)
{
	putchar('\n');
	printf("单链表测试样例:\n");
	Slist nlist;
    int num;
    sInit(&nlist); //初始化
    for(num = 1; num < 10; num++)
    {
        sInsert(&nlist, num);  //插入
    }
    sTreaverse(&nlist, printData);//遍历打印data
    sRemove(&nlist, 3); //移除
    printf("\n");
    sTreaverse(&nlist, printData);
    sModify(&nlist, 8, 44);//修改
    printf("\n");
    sTreaverse(&nlist, printData);
}


