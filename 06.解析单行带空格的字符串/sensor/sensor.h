#ifndef __SENSOR_H
#define __SENSOR_H
#include "main.h"

/*传感器版本*/
/*
	0：双通道
	1: 单通道带光强
	2: 但通道不带光强
*/
#define SENSOR_VERSION 0

/*传感器数据结构*/
typedef struct
{
    int Serial_Number 	 ;	/*序号*/
    int Tp1_Signal_Value ;	/*通道1信号值*/
    int Tp1_Devalue 	 ;	/*通道1差值*/
    int Tp2_Signal_Value ;	/*通道2信号值*/
    int Tp2_Devalue 	 ;	/*通道2差值*/
    int Tp1_Light_Value  ;	/*通道1光强*/
    int Tp2_Light_Value  ;	/*通道2光强*/
} Sensor_Data_Format      ;
extern __IO Sensor_Data_Format sensor_data ;


/*解析双通道数据格式结构体*/
typedef struct
{
    int Para1 ;
    int Para2 ;
    int Para3 ;
    int Para4 ;
    int Para5 ;
    int Para6 ;
    int Para7 ;
} Parse_Double_Passage_Data_Format ;

/*解析单通道带光强数据格式结构体*/
typedef struct
{
    int Para1 ;
    int Para2 ;
    int Para3 ;
    int Para4 ;
} Parse_Single_Passage_Have_Light_Value ;

/*解析单通道不带光强数据格式结构体*/
typedef struct
{
    int Para1 ;
    int Para2 ;
    int Para3 ;
} Parse_Single_Passage_DHave_Light_Value ;

/*获取传感器数据*/
void Get_Sensor_Data(char *data, Sensor_Data_Format *sensor_data);


#endif //__SENSOR_H

