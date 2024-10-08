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

///////////////////////////////////////
/* 宏定义区域 */
#define USART2_BUF_LEN 		64 				//定义 UART4 缓冲长度为 64 个字节
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 		1 				//0,不接收;1,接收.

///////////////////////////////////////
/* 外部变量申明区 */
extern uint8_t	USART2_RX_BUF[USART2_BUF_LEN];	//接收缓存，最大USART_REC_LEN个字节，末字节为换行符
extern uint8_t	ReceiveState2;					//接收状态标记
extern uint16_t	RxCounter2;						//接收长度

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitUart2(uint32_t bound);
void Uart2_STA_Clr(void);
void USART2_Send_Data(uint8_t *buf,uint8_t len);

#endif
