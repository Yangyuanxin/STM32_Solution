
#include "link.h"

NODE_T  *Create_link_header(int size)
{
	NODE_T *tmpHeader = NULL ;
	
	tmpHeader = malloc(sizeof(NODE_T));
	if(NULL == tmpHeader)
	{
		fprintf(stderr,"malloc header fail \n");
		return NULL ;
	}
	
	tmpHeader->size = size ; 
	tmpHeader->ptr = NULL ;
	tmpHeader->next = NULL ; 
	return  tmpHeader ; 
}
void top_append(NODE_T *Header , void *Data , int size)
{
	NODE_T *New = NULL ; 

	New = malloc(sizeof(NODE_T));	
	if(NULL == New)
	{
		fprintf(stderr , "malloc node fail\n");
		return ;
	}

	New->size = size ; 
	New->ptr = malloc(size);	
	if(NULL == New->ptr)
	{
		free(New);
		fprintf(stderr , "malloc ptr fail \n");
		return ; 
	}

	memcpy(New->ptr , Data, size);

	
	New->next = Header->next ; 
	Header->next = New ; 
}

void traver_link(NODE_T *Header , print_t  func)
{
	NODE_T *tmp = Header->next ; 
	while(tmp)
	{
		func(tmp->ptr);
		tmp = tmp->next ; 
	}
}

void tail_append(NODE_T *Header , void *Data , int size)
{
	NODE_T *New = NULL ; 	
	
	New = malloc(sizeof(NODE_T));
	if(NULL == New)	
	{
		fprintf(stderr , "malloc tail append node error\n");
		return  ;
	}
//	New->next = NULL ; 
	New->size = size ; 
	New->ptr = malloc(size);
	if(NULL == New->ptr)
	{
		fprintf(stderr , "malloc tail append ptr fail\n");
		return  ;
	}

	memcpy(New->ptr , Data , size);

	NODE_T *tmp = Header; 
	while(tmp->next)
	{
		tmp = tmp->next ; 
	}
	
	New->next = tmp->next ; 
	tmp->next = New ; 
}	

//删除 按条件 ＝＝＞ 用户规定
int  delete_link(NODE_T *Header , delete_t func , void *Con)
{
	NODE_T *first = Header, *second = Header->next ; 

	while(second)
	{
		if(func(second , Con))
		{
			first->next = second->next ;
			free(second);
			second = first->next ;
			continue ;
		}
		first = second ; 
		second = second->next ; 
	}
}

//插入 条件插入
int  insert_link(NODE_T *Header , delete_t func , void *Con , void *Data , int size)
{
	NODE_T *second = Header->next ; 
	NODE_T *New = NULL ; 	

	while(second)
	{
		if(func(second , Con))
		{
			New = malloc(sizeof(NODE_T));
			if(NULL == New)	
			{
				fprintf(stderr , "malloc tail append node error\n");
				return  ;
			}
		//	New->next = NULL ; 
			New->size = size ; 
			New->ptr = malloc(size);
			if(NULL == New->ptr)
			{
				fprintf(stderr , "malloc tail append ptr fail\n");
				return  ;
			}
		
			memcpy(New->ptr , Data , size);
	
			New->next = second->next ; 
			second->next = New ;
			second = second->next ; 
		}
		second = second->next ; 
	}

}
//返回找到元素的个数 
int find_link(NODE_T *Header , delete_t func , void *Con)
{
	NODE_T *tmp = Header->next ;
	int count = 0 ; 
	while(tmp)
	{
		if(func(tmp , Con))	
		{
			count++ ; 
		}
		tmp = tmp->next ;
	}
	return count ; 
}

void turn_over_link(NODE_T *Header)
{
	NODE_T *link = Header->next ,*cur ; 
	Header->next = NULL ;

	while(link)
	{	
		cur = link ;
		link = link->next ;
	
		cur->next = Header->next ;
		Header->next = cur ; 
	}
}

int  sort_select(NODE_T *Header , compar_t func)
{
	NODE_T *First = Header->next ; 
	NODE_T *Second  = NULL ;	
	NODE_T tmp ; 
	NODE_T *tmp1 = NULL , *tmp2 = NULL ; 

	while(First)
	{
		Second = First->next;
		while(Second)
		{
			if(func(First , Second) > 0)
			{
				tmp1 = First->next ;
				tmp2 = Second->next ; 

				memcpy(&tmp , First , sizeof(NODE_T));
				memcpy(First , Second , sizeof(NODE_T));
				memcpy(Second , &tmp , sizeof(NODE_T));
				
				First->next	 = tmp1 ; 
				Second->next = tmp2 ; 
			}
			Second = Second->next ; 
		}
		First = First->next ;
	}
}

void save_file(NODE_T *Header , const char *Path)
{
	FILE *filp = NULL ;

	if(strlen(Path) == 0)
		return ; 

	filp = fopen(Path , "w");
	if(NULL == filp)
	{
		fprintf(stderr , "Open file :%s fail \n" , Path);
		return ; 
	}

	NODE_T *tmp = Header->next ; 

	while(tmp)
	{
		fwrite(tmp , sizeof(NODE_T) , 1 , filp);
		fwrite(tmp->ptr , tmp->size , 1 , filp);
		tmp = tmp->next ; 
	}

	fclose(filp);
}

void  load_file(NODE_T *Header , const char *Path)
{
	FILE *filp = NULL ; 

	if(strlen(Path) == 0)
		return ; 

	filp = fopen(Path , "r");
	if(NULL == filp)
	{
		fprintf(stderr , "open file : %s fail \n" , Path);
		return  ; 
	}



	NODE_T *New = NULL ;	
	int ret ; 
	NODE_T *tmp = Header; 
	while(1)
	{
		New = malloc(sizeof(NODE_T));
		if(NULL == New)
		{
			fprintf(stderr , "malloc NODE_T fail \n");
			return ; 
		}
	
		ret = fread(New , sizeof(NODE_T) , 1 , filp);
		if(ret != 1)
		{
			free(New);
			break;
		}
		New->ptr = malloc(New->size);
		if(NULL == New->ptr)
		{
			fprintf(stderr , "malloc ptr fail \n");
			return ; 
		}
		
		ret = fread(New->ptr , New->size , 1 , filp);
		if(ret != 1)
		{	
			free(New->ptr);
			free(New);
			break;
		}

		while(tmp->next)		
		{
			tmp = tmp->next ;
		}
		
		New->next = tmp->next ; 
		tmp->next = New ; 
	}
}


