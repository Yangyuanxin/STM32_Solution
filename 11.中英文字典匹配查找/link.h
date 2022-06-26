
#ifndef   _LINK_H
#define   _LINK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef  struct  node
{
	int size ;         //Data Type Size数据类型大小
	void *ptr ; 
	struct  node *next ; 
}NODE_T;

typedef   void (*print_t)(void *Data);
typedef   int  (*delete_t)(void *Data , void *Con);
typedef   int  (*compar_t)(void *First , void *Second);

NODE_T  *Create_link_header(int size);

void top_append(NODE_T *Header , void *Data , int size);

void traver_link(NODE_T *Header , print_t  func);

//尾插
void tail_append(NODE_T *Header , void *Data , int size);

//删除 按条件 ＝＝＞ 用户规定
int  delete_link(NODE_T *Header , delete_t func , void *Con);

//插入 条件插入
int  insert_link(NODE_T *Header , delete_t func , void *Con , void *Data , int size);
//查找 条件查找
int find_link(NODE_T *Header , delete_t func , void *Con);

//翻转 
void turn_over_link(NODE_T *Header);

//sort  排序     由用户规定的条件
int sort_select(NODE_T *Header , compar_t func);

//保存文件    格式用户规定
void save_file(NODE_T *Header , const char *Path);

//导放文件    格式用户规定
void load_file(NODE_T *Header , const char *Path);

#endif   //_LINK_H
