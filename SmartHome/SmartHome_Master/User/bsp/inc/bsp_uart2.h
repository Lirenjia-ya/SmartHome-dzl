/*
*********************************************************************************************************
*
*	ģ������ : ģ��
*	�ļ����� : bsp_uart2.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef	__BSP_UART2_H
#define	__BSP_UART2_H

#include "bsp.h"

#define	USART2_BUF_LEN 64	//����USART2���峤��λ64���ֽ�
extern	uint8_t	USART2_RX_BUF[USART2_BUF_LEN];	//���ջ��壬���USART2_BUF_LEN���ֽ�
extern	uint8_t	USART2_RX_CNT;					//���յ������ݳ���

//����봮���жϽ��գ��벻Ҫע��һ�º궨��
#define	EN_USART2_RX 	1			//0,�����ܣ�1����

void bsp_InitUart2(uint32_t bound);
void USART2_Send_Data(uint8_t *buf,uint8_t len);
void USART2_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
