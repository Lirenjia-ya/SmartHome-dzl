/*
*********************************************************************************************************
*
*	ģ������ : ģ��
*	�ļ����� : bsp_uart3.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef	__BSP_UART3_H
#define	__BSP_UART3_H

#include "bsp.h"

#define	UART3_BUF_LEN 64	//����UART3���峤��λ64���ֽ�

//����봮���жϽ��գ��벻Ҫע��һ�º궨��
#define	EN_UART3_RX 	1			//0,�����ܣ�1����


extern	uint8_t	UART3_RX_BUF[UART3_BUF_LEN];	//���ջ��壬���UART3_BUF_LEN���ֽ�
extern	uint8_t	UART3_RX_CNT;					//���յ������ݳ���


void bsp_InitUart3(uint32_t bound);
void UART3_Send_Data(uint8_t *buf,uint8_t len);
void UART3_Receive_Data(uint8_t *buf,uint8_t *len);

#endif
