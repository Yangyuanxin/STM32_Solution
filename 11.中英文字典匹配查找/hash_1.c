
#include "link.h"
#include <sys/time.h>

struct ciku
{
	char En[10] ; 
	char Ch[1024] ; 
};

int  find(void *Data , void *Con);
void get_index(char *En , int size , int *indexrow , int *indexcol);

int main(void)
{
	//申请长度为26的头
	NODE_T *array[26][26] = {0};
	struct timeval tv ; 
	struct timezone  tz ; 

	int i  , j; 
	//初始化头
	for(i = 0 ; i < 26 ; i++)
	{
		for(j = 0 ; j < 26 ; j++)
		{
			array[i][j] = Create_link_header(0);
			if(NULL == array[i][j])
			{
				fprintf(stderr , "create header fail \n");
				return -1 ; 
			}	
		}
	}

	FILE *filp = NULL ; 

	filp = fopen("zk.txt" , "r");
	if(NULL == filp)
	{
		fprintf(stderr , "open ciku.dict fail \n");
		return -2 ; 
	}
	
		gettimeofday(&tv , &tz);
		printf("before loading tv.sec:%d  tv.usec:%d \n" , tv.tv_sec , tv.tv_usec);	

	struct ciku  ci ; 
	char Ch[200] = {0};
	char *p = NULL ;
	int indexrow = 0 ;
	int indexcol = 0 ;  
	while(1)
	{
		p = fgets(Ch , sizeof(Ch) , filp);
		if(NULL == p)
			break;
		Ch[strlen(Ch)-1] = '\0' ;

		strcpy(ci.Ch , Ch);			
	
		get_index(ci.En , strlen(ci.En) , &indexrow , &indexcol);

//		tail_append(array[index] , &ci , sizeof(struct ciku));
		tail_append(array[indexrow][indexcol] , &ci , sizeof(struct ciku));
	}

	char En[200]={0} ; 
		gettimeofday(&tv , &tz);
		printf("after loading tv.sec:%d  tv.usec:%d \n" , tv.tv_sec , tv.tv_usec);	
	
	printf("请输入一个单词:\n");
	gets(En);
		gettimeofday(&tv , &tz);
		printf("before search tv.sec:%d  tv.usec:%d \n" , tv.tv_sec , tv.tv_usec);	

	get_index(En , strlen(En) , &indexrow , &indexcol);	

	find_link(array[indexrow][indexcol] , find , (void *)En);

		gettimeofday(&tv , &tz);
		printf("after search tv.sec:%d  tv.usec:%d \n" , tv.tv_sec , tv.tv_usec);	

	return 0 ;
}
int  find(void *Data , void *Con)
{
	NODE_T *data = Data ; 
	
	char *con = Con ; 
	struct ciku *ci = data->ptr ;

	//完全匹配
	if(strcmp(ci->En , con) == 0)
	{
		printf("查找成功 ：\n");
		printf("En:%s\n" , ci->En);
		printf("Ch:%s\n" , ci->Ch);
		return 1 ; 
	}
	return 0 ; 
}
void get_index(char *En , int size , int *indexrow , int *indexcol)
{

		if((En[0] >= 'a') &&(En[0] <= 'z'))	
			*indexrow = En[0]-'a' ; 
		else if((En[0] >= 'A') &&(En[0] <= 'Z'))	
			*indexrow = En[0]-'A' ; 
		else
			*indexrow = 0 ; 

		if(size > 1)
		{
			if((En[1] >= 'a') &&(En[1] <= 'z'))	
				*indexcol = En[1]-'a' ; 
			else if((En[1] >= 'A') &&(En[1] <= 'Z'))	
				*indexcol = En[1]-'A' ; 
			else
				*indexcol = 0 ; 
		}
		else
			*indexcol = 0 ; 
}
