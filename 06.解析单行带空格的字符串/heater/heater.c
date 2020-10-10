#include "heater.h"

__IO Heater_Data_Format heater_data ;

/*解析加热器数据*/
static void Handler_Heater_Data(char *Data,Parse_Heater_Data *Heater_Data_Info)
{
	char *temp = NULL ;
	Heater_Data_Info->Para1 = atoi(Data);
	temp = strstr(Data," ");
	Heater_Data_Info->Para2 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Heater_Data_Info->Para3 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Heater_Data_Info->Para4 = atoi(temp+1);
	temp = strstr(temp+1," ");
}

/*获取加热器数据*/
void Get_Heater_Data(char *data,Heater_Data_Format *heater_data)
{
	Parse_Heater_Data para ;
	Handler_Heater_Data(data,&para);
	heater_data->Heater_Serial_Number = para.Para1 ;
	heater_data->Heater_Signal_Value = para.Para2 ;
	heater_data->Heater_Devalue = para.Para3 ;
	heater_data->Heater_Status_Bit = para.Para4 ;
}

