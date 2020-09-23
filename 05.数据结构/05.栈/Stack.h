#ifndef __STACK_H
#define __STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

#define STACK_INIT_SIZE 10 //存储空间的初始化分配
#define STACK_INCREMENT 10 //增量

typedef int SElemTpye; //元素类型
typedef int Status; //元素类型

typedef struct
{
   SElemTpye *base;//栈底指针
   SElemTpye *top;//栈顶指针
   int size;
} Stack;

Status Stack_Init(Stack *stack);
Status Stack_Push(Stack *s, SElemTpye e);
Status Stack_Pop(Stack *s, SElemTpye *e);
Status Stack_Destroy(Stack *s);
int Test_Stack_Sample(void);


#endif //__STACK_H

