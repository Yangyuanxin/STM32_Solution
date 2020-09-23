#include "Stack.h"

Status Stack_Init(Stack *stack)
{
    stack->base = (SElemTpye *) malloc(sizeof(SElemTpye) * STACK_INIT_SIZE);
    if(!stack->base) return ERROR;
    stack->top = stack->base;
    stack->size = STACK_INIT_SIZE;
    return OK;
}

Status Stack_Push(Stack *s, SElemTpye e)
{
    if(s->top - s->base >= s->size)
    {
        s->base = (SElemTpye *) realloc( s->base, (s->size + STACK_INCREMENT) * sizeof(SElemTpye));
        if(!s->base) return ERROR;
        s->top = s->base + s->size;
        s->size += STACK_INCREMENT;
    }
    *s->top++ = e;
    return OK;
}

Status Stack_Pop(Stack *s, SElemTpye *e)
{
    if(s->top == s->base) return ERROR;
    *e = * --s->top;
    return OK;
}

Status Stack_Destroy(Stack *s)
{
    free((Stack *)s);
	return OK ;
}


int Test_Stack_Sample(void)
{
	Stack s;
	int e = 0 ;
	int i = 0 ;
	int rand_data = 0 ;
	Stack_Init(&s);
	//进栈
	printf("\n进栈:\n");
    for (i = 0; i < 10; ++i){
		rand_data = rand()%(55000-46000)+46000; /*生成一个bai[t,d)区间内的du整数*/
        Stack_Push(&s, rand_data);
		printf("%d ",rand_data);
	}
	putchar('\n');
	printf("出栈:\n");
    for (i = 0; i < 10; ++i){
        Stack_Pop(&s, &e);
		printf("%d ",e);
	}
	Stack_Destroy(&s);
	return OK ;
}
