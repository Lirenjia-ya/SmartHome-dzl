/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : bsp_iosensor.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-04-12
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_IOSENSOR_H_
#define _BSP_IOSENSOR_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

////////////////////////
//��ֲ�޸���
//����ʵ�ʶ˿��޸�
//��ǰ�汾����������ֲ��ע�⣬�˿ڽ��������ڷ�JTAG/SWD���ţ������JTSG���ţ���Ҫ��AFIOʱ�ӣ���ʧ��JTAG��


////////////////////////

///////////////////////////////////////
/* �궨������ */
//DO101 �𶯴�����
#define VIBRATE_OUT_GPIO_PIN		GPIO_Pin_11					//VIBRATE_OUT���ź�
#define VIBRATE_OUT_PIN_ID			11							//VIBRATE_OUT�������
#define VIBRATE_OUT_GPIO_PORT		GPIOA						//VIBRATE_OUT�˿ں�
#define VIBRATE_OUT_GPIO_CLK		RCC_APB2Periph_GPIOA		//VIBRATE_OUTʱ��
//#define VIBRATE_OUT_FUN_OUT			PAout						//VIBRATE_OUT����˿����ú���
#define VIBRATE_OUT_FUN_IN			PAin						//VIBRATE_OUT����˿����ú���
#define VIBRATE_OUT_GPIO_MODE		GPIO_Mode_IPU				//VIBRATE_OUT�ⲿ�ж����뺯��
#define VIBRATE_OUT_ACTIVE_LEVEL	0							//VIBRATE_OUT��Ч��ƽΪ�͵�ƽ

#define VIBRATE_OUT_PORT_SRC		GPIO_PortSourceGPIOA		//VIBRATE_OUT�ⲿ�ж϶˿ں�
#define VIBRATE_OUT_PIN_SRC			GPIO_PinSource11			//VIBRATE_OUT�ⲿ�ж����ź�
#define VIBRATE_OUT_EXTI_LINE		EXTI_Line11					//VIBRATE_OUT�ⲿ�жϺ�
#define VIBRATE_OUT_EXTI_TRIG		EXTI_Trigger_Falling		//VIBRATE_OUT�ⲿ�жϴ�����ʽ
#define VIBRATE_OUT_EXTI_IRQN		EXTI15_10_IRQn				//VIBRATE_OUT�ⲿ�ж�NVIC��

////////////////////////
//PIR ������⴫����
#define PIR_OUT_GPIO_PIN			GPIO_Pin_3					//PIR_OUT���ź�
#define PIR_OUT_PIN_ID				3							//PIR_OUT�������
#define PIR_OUT_GPIO_PORT			GPIOB						//PIR_OUT�˿ں�
#define PIR_OUT_GPIO_CLK			RCC_APB2Periph_GPIOB		//PIR_OUTʱ��
//#define PIR_OUT_FUN_OUT				PBout						//PIR_OUT����˿����ú���
#define PIR_OUT_FUN_IN				PBin						//PIR_OUT����˿����ú���
#define PIR_OUT_GPIO_MODE			GPIO_Mode_IPD				//PIR_OUT�ⲿ�ж����뺯��
#define PIR_OUT_ACTIVE_LEVEL		1							//PIR_OUT��Ч��ƽΪ�ߵ�ƽ

#define PIR_OUT_PORT_SRC			GPIO_PortSourceGPIOB		//PIR_OUT�ⲿ�ж϶˿ں�
#define PIR_OUT_PIN_SRC				GPIO_PinSource3			//PIR_OUT�ⲿ�ж����ź�
#define PIR_OUT_EXTI_LINE			EXTI_Line3					//PIR_OUT�ⲿ�жϺ�
#define PIR_OUT_EXTI_TRIG			EXTI_Trigger_Rising			//PIR_OUT�ⲿ�жϴ�����ʽ
#define PIR_OUT_EXTI_IRQN			EXTI15_10_IRQn				//PIR_OUT�ⲿ�ж�NVIC��

////////////////////////
//IO��������
#define VIBRATE_OUT			VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID)		//��ȡVIBRATE_OUT �͵�ƽ��Ч
#define PIR_OUT				PIR_OUT_FUN_IN(PIR_OUT_PIN_ID)				//��ȡPIR_OUT �ߵ�ƽ��Ч

///////////////////////////////////////
/* �ⲿ���������� */
extern uint8_t VIBRATE_OUT_flag;
extern uint8_t PIR_OUT_flag;

///////////////////////////////////////
/* ���������� */
void bsp_InitIOSensor(void);

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

