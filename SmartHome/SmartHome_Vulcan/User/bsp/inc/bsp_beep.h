/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : bsp_beep.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����  240301      ����   ������  ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_BEEP_H_
#define _BSP_BEEP_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

///////////////////////////////////////
/* �궨������ */
#define RCC_ALL_BEEP 	(BEEP_GPIO_CLK)

#define BEEP_GPIO_PIN		GPIO_Pin_11				//BEEP���ź�
#define BEEP_PIN_ID			11						//BEEP�������
#define BEEP_GPIO_PORT		GPIOG					//BEEP�˿ں�
#define BEEP_GPIO_CLK		RCC_APB2Periph_GPIOG	//BEEPʱ��
#define BEEP_FUN_OUT		PGout					//BEEP����˿����ú���
//#define BEEP_FUN_IN			PGin					//BEEP����˿����ú���

//IO��������	 
#define BEEP    BEEP_FUN_OUT(BEEP_PIN_ID) //BEEP


///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
void bsp_InitBeep(void);
void bsp_BeepOn(void);
void bsp_BeepOff(void);
void bsp_BeepToggle(void);
uint8_t bsp_IsBeepOn(void);

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

