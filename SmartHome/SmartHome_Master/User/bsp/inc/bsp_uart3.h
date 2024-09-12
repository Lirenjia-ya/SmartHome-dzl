/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : bsp_uart3.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef	__BSP_UART3_H
#define	__BSP_UART3_H

#include "bsp.h"

#define	UART3_BUF_LEN 64	//定义UART3缓冲长度位64个字节

//如果想串口中断接收，请不要注释一下宏定义
#define	EN_UART3_RX 	1			//0,不接受，1接收


extern	uint8_t	UART3_RX_BUF[UART3_BUF_LEN];	//接收缓冲，最大UART3_BUF_LEN个字节
extern	uint8_t	UART3_RX_CNT;					//接收到的数据长度


void bsp_InitUart3(uint32_t bound);
void UART3_Send_Data(uint8_t *buf,uint8_t len);
void UART3_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
