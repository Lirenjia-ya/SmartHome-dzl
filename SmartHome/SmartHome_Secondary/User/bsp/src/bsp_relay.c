/*
*********************************************************************************************************
*
*	ģ������ : �̵�������ģ��
*	�ļ����� : bsp_Relay.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-05-29  waroyal  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitRelay
*	����˵��: ���ü̵�����ص�GPIO
*	��    ��: void
*	�� �� ֵ: void
*********************************************************************************************************
*/
void bsp_InitRelay(void)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY,ENABLE);
	
	//��ʼ��Ϊ�Ͽ�
	bsp_RelayOff(1);
	bsp_RelayOff(2);
	bsp_RelayOff(3);
	bsp_RelayOff(4);
	
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Relay1
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;
	GPIO_Init(RELAY1_GPIO_PORT,&GPIO_InitStructure);
	//Relay2
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;
	GPIO_Init(RELAY2_GPIO_PORT,&GPIO_InitStructure);
	//Relay3
	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;
	GPIO_Init(RELAY3_GPIO_PORT,&GPIO_InitStructure);
	//Relay4
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;
	GPIO_Init(RELAY4_GPIO_PORT,&GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayOn
*	����˵��: �պϼ̵���
*	��    ��: uint8_t _no:�̵������
*	�� �� ֵ: void
*********************************************************************************************************
*/
void bsp_RelayOn(uint8_t _no)
{
	if(_no==1)
	{
		RELAY1=1;
	}
	else if(_no==2)
	{
		RELAY2=1;
	}
	else if(_no==3)
	{
		RELAY3=1;
	}
	else if(_no==4)
	{
		RELAY4=1;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayOff
*	����˵��: �Ͽ��̵���
*	��    ��: uint8_t _no:�̵������
*	�� �� ֵ: void
*********************************************************************************************************
*/
void bsp_RelayOff(uint8_t _no)
{
	if(_no==1)
	{
		RELAY1=0;
	}
	else if(_no==2)
	{
		RELAY2=0;
	}
	else if(_no==3)
	{
		RELAY3=0;
	}
	else if(_no==4)
	{
		RELAY4=0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RelayToggle
*	����˵��: ��תָ���̵���
*	��    ��: uint8_t _no:�̵������
*	�� �� ֵ: void
*********************************************************************************************************
*/
void bsp_RelayToggle(uint8_t _no)
{
	if(_no==1)
	{
		RELAY1_GPIO_PORT->ODR ^= RELAY1_GPIO_PIN;
	}
	else if(_no==2)
	{
		RELAY2_GPIO_PORT->ODR ^= RELAY2_GPIO_PIN;
	}
	else if(_no==3)
	{
		RELAY3_GPIO_PORT->ODR ^= RELAY3_GPIO_PIN;
	}
	else if(_no==4)
	{
		RELAY4_GPIO_PORT->ODR ^= RELAY4_GPIO_PIN;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsRelayOn
*	����˵��: �жϼ̵����Ƿ�պ�
*	��    ��: uint8_t _no:�̵������
*	�� �� ֵ: 1-�պ� 0-�Ͽ�
*********************************************************************************************************
*/
uint8_t bsp_IsRelayOn(uint8_t _no)
{
	if(_no==1)
	{
		if((RELAY1_GPIO_PORT->ODR&RELAY1_GPIO_PIN)==RELAY1_GPIO_PIN) return 1;
		return 0;
	}
	else if(_no==2)
	{
		if((RELAY2_GPIO_PORT->ODR&RELAY2_GPIO_PIN)==RELAY2_GPIO_PIN) return 1;
		return 0;
	}
	else if(_no==3)
	{
		if((RELAY3_GPIO_PORT->ODR&RELAY3_GPIO_PIN)==RELAY3_GPIO_PIN) return 1;
		return 0;
	}
	else if(_no==4)
	{
		if((RELAY4_GPIO_PORT->ODR&RELAY4_GPIO_PIN)==RELAY4_GPIO_PIN) return 1;
		return 0;
	}
	
	return 0;
}








///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
