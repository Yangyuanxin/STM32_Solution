#include "sensor.h"


/*传感器数据*/
__IO Sensor_Data_Format sensor_data ;



/*解析双通道数据*/
static void Handler_Double_Passage_Data(char *Data,Parse_Double_Passage_Data_Format *Sensor_Data_Info)
{
	char *temp = NULL ;
	Sensor_Data_Info->Para1 = atoi(Data);
	temp = strstr(Data," ");
	Sensor_Data_Info->Para2 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para3 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para4 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para5 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para6 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para7 = atoi(temp+1);
	temp = strstr(temp+1," ");
}



/*解析单通道带光强数据*/
static void Handler_Single_Passage_Have_Light_Data(char *Data,Parse_Single_Passage_Have_Light_Value *Sensor_Data_Info)
{
	char *temp = NULL ;
	Sensor_Data_Info->Para1 = atoi(Data);
	temp = strstr(Data," ");
	Sensor_Data_Info->Para2 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para3 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para4 = atoi(temp+1);
	temp = strstr(temp+1," ");
}


/*解析单通道不带光强数据*/
static void Handler_Single_Passage_DHave_Light_Data(char *Data,Parse_Single_Passage_DHave_Light_Value *Sensor_Data_Info)
{
	char *temp = NULL ;
	Sensor_Data_Info->Para1 = atoi(Data);
	temp = strstr(Data," ");
	Sensor_Data_Info->Para2 = atoi(temp+1);
	temp = strstr(temp+1," ");
	Sensor_Data_Info->Para3 = atoi(temp+1);
	temp = strstr(temp+1," ");
}

/*获取传感器数据*/
void Get_Sensor_Data(char *data,Sensor_Data_Format *sensor_data)
{
	Parse_Double_Passage_Data_Format para_0 ;
	Parse_Single_Passage_Have_Light_Value para_1 ;
	Parse_Single_Passage_DHave_Light_Value para_2 ;
	switch(SENSOR_VERSION)
	{
		case 0:
			Handler_Double_Passage_Data(data,&para_0);
			sensor_data->Serial_Number 	  = para_0.Para1 ;
			sensor_data->Tp1_Signal_Value = para_0.Para2 ;
			sensor_data->Tp1_Devalue	  = para_0.Para3 ;
			sensor_data->Tp2_Signal_Value = para_0.Para4 ;
			sensor_data->Tp2_Devalue	  = para_0.Para5 ;
			sensor_data->Tp1_Light_Value  = para_0.Para6 ;
			sensor_data->Tp2_Light_Value  = para_0.Para7 ;
			break ;
		case 1:
			Handler_Single_Passage_Have_Light_Data(data,&para_1);
			sensor_data->Serial_Number 	  = para_1.Para1 ;
			sensor_data->Tp1_Signal_Value = para_1.Para2 ;
			sensor_data->Tp1_Devalue	  = para_1.Para3 ;
			sensor_data->Tp1_Light_Value  = para_1.Para4 ;
			break ;
		case 2:
			Handler_Single_Passage_DHave_Light_Data(data,&para_2);
			sensor_data->Serial_Number 	  = para_2.Para1 ;
			sensor_data->Tp1_Signal_Value = para_2.Para2 ;
			sensor_data->Tp1_Devalue	  = para_2.Para3 ;
			break ;
		default:
			break ;
	}
}

