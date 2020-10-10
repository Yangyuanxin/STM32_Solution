#ifndef __HEATER_H
#define __HEATER_H
#include "main.h"

#define STATUS_1 1	/*加热未稳定*/
#define STATUS_2 2	/*加热未稳定且有纸插入*/
#define STATUS_3 3	/*加热稳定*/
#define STATUS_4 4	/*加热稳定且有纸插入*/

/*加热器数据结构*/
typedef struct
{
    int Heater_Serial_Number 	 ;	/*加热器序号*/
    int Heater_Signal_Value  	 ;	/*加热器信号值*/
    int Heater_Devalue 		 	 ;	/*加热器差值*/
    int Heater_Status_Bit 	 	 ;	/*加热器状态位*/
} Heater_Data_Format			 ;
extern __IO Heater_Data_Format heater_data ;

/*解析加热器数据格式结构体*/
typedef struct
{
    int Para1 ;
    int Para2 ;
    int Para3 ;
    int Para4 ;
} Parse_Heater_Data ;

/*获取加热器数据*/
void Get_Heater_Data(char *data, Heater_Data_Format *heater_data);

#endif //__HEATER_H
