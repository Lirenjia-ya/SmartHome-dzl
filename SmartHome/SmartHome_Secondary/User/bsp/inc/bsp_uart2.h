/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : bsp_uart2.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef	__BSP_UART2_H
#define	__BSP_UART2_H

#include "bsp.h"

#define	USART2_BUF_LEN 64	//定义USART2缓冲长度位64个字节
extern	uint8_t	USART2_RX_BUF[USART2_BUF_LEN];	//接收缓冲，最大USART2_BUF_LEN个字节
extern	uint8_t	USART2_RX_CNT;					//接收到的数据长度

//如果想串口中断接收，请不要注释一下宏定义
#define	EN_USART2_RX 	1			//0,不接受，1接收

void bsp_InitUart2(uint32_t bound);
void USART2_Send_Data(uint8_t *buf,uint8_t len);
void USART2_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
