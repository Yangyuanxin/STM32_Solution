#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HASHSIZE 101

//哈希表元素
typedef struct nodelist
{
    struct nodelist *next; //链表法的链表
    char *key; //哈希的key
    char *value; //哈希的值
} Nodelist;

//哈希表
static Nodelist *hashTable[HASHSIZE]; 


//散列函数--除法散列法
unsigned hash(char *key);

//查询key
Nodelist *search(char *key);

//插入到哈希表
Nodelist *insert_hash_table(char *key, char *value);

//根据key获取值
char * get_value(char *key);

void hash_table_sample(void);

#endif //__HASH_TABLE_H
