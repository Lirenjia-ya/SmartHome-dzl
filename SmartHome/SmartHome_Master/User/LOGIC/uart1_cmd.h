/*
*********************************************************************************************************
*
*	模块名称 : usart1_cmd模块
*	文件名称 : usart1_cmd.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef __UART1_CMD_H_
#define __UART1_CMD_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

///////////////////////////////////////
/* 宏定义区域 */
//帧头
#define	FRAME_HEADER_BYTE_1				0x41		//A
#define	FRAME_HEADER_BYTE_2				0x73		//s
#define	FRAME_HEADER_BYTE_3				0x75		//u
#define	FRAME_HEADER_BYTE_4				0x6b		//k
#define	FRAME_HEADER_BYTE_5				0x61		//a

//数据区的长度
#define	FRAME_CONST_BYTE_LEN			(5+1+1)	//非数据区的数据长度 帧头+数据长度+校验码

//数据区
#define	FRAME_DATA_FIELD_OFFSET		6				//数据区的开头偏移地址 也就是地址码

//地址码
#define	ADDRESS_ROOM							0x01		//主卧

//命令
#define	FUN_CODE_DEVICE_RELAY			0x01		//继电器命令
#define	FUN_CODE_DEVICE_FAN				0x02		//风扇命令

//继电器号
#define	FUN_DEVICE_RELAY1					0x01		//继电器1
#define	FUN_DEVICE_RELAY2					0x02		//继电器2
#define	FUN_DEVICE_RELAY3					0x03		//继电器3
#define	FUN_DEVICE_RELAY4					0x04		//继电器4

//风扇号
#define	FUN_DEVICE_FAN1						0x01		//风扇1
#define	FUN_DEVICE_FAN2						0x02		//风扇2

//具体命令
#define	FUN_CODE_CMD_RELAY_TOGGLE	0x01		//继电器翻转命令
#define	FUN_CODE_CMD_FAN_TOGGLE		0x01		//风扇翻转命令

//继电器状态
#define	FUN_CODE_RSP_RELAY_OFF		0x01		//继电器关
#define	FUN_CODE_RSP_RELAY_ON			0x00		//继电器开

//风扇状态
#define	FUN_CODE_RSP_FAN_OFF			0x01		//风扇关
#define	FUN_CODE_RSP_FAN_ON				0x00		//风扇开


//数据帧		
#define FUN_CODE_DATA 										0x01		//功能码 发送传感器数据
//单个传感器的数据长度
#define SENSOR_SINGLE_DATA_LEN 						8

#define DATA_LEN_OFFSET 									5 			//数据长度偏移地址
#define FRAME_TYPE_OFFSET 								6 			//帧类型偏移地址
#define DATA_TYPE_OFFSET 									7 			//数据帧的数据类型偏移地址
#define SENSOR_DATA_LEN_OFFSET 						8 			//传感器数据长度 偏移地址
#define SENSOR_DATA_START_OFFSET 					9 			//第一个传感器数据首地址 偏移地址
#define SENSOR_TEMP_CODE 									0x01 		//温度传感器
#define SENSOR_HUMI_CODE 									0x02		//湿度传感器
#define SENSOR_LM_CODE 										0x04 		//光照传感器
#define SENSOR_CO2_CODE 									0x05 		//CO2 传感器
#define SENSOR_PM25_CODE 									0x06 		//PM2.5 传感器

#define SENSOR_SHOCK_CODE 								0x07 		//震动传感器 & 红外传感器

#define ENVIRONMENT_SENSOR_TYPE_CODE_MSB 	0x22 		//环境传感器
#define ENVIRONMENT_SENSOR_TYPE_CODE_LSB 	0x00	 	//
#define SENSOR_STATUS_PRE 								0x00	 	//传感器预热
#define SENSOR_STATUS_NG 									0x01 		//传感器数值异常
#define SENSOR_STATUS_RUN 								0x02 		//传感器正常运行
#define DATA_TYPE_SENSOR 									0x01 		//传感器数据

//注意 x[3]为大端,x[0]为小端
typedef union
{
	float f;
	uint8_t x[4];
}_sensorData;//传感器数据 float 与 4 字节 相互变换 使用联合体

///////////////////////////////////////
/* 外部变量申明区 */
extern uint8_t SENSOR_DATA_BYTE[248];

///////////////////////////////////////
/* 函数申明区 */
void actuator_Data_Receive(void);
uint8_t CheckSum(uint8_t USART_RX_BUF[],uint8_t len);

//字节形式传输温湿度信息
void sensor_Data_Upload(float temperature,float humidity,float co2,float pm25,float light);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

