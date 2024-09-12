/*
*********************************************************************************************************
*
*	模块名称 : 继电器驱动模块
*	文件名称 : bsp_Relay.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-05-29  waroyal  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

///////////////////////////////////////
/* 变量定义区 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitRelay
*	功能说明: 配置继电器相关的GPIO
*	形    参: void
*	返 回 值: void
*********************************************************************************************************
*/
void bsp_InitRelay(void)
{
	//1.定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.打开时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_RELAY,ENABLE);
	
	//初始化为断开
	bsp_RelayOff(1);
	bsp_RelayOff(2);
	bsp_RelayOff(3);
	bsp_RelayOff(4);
	
	//3.结构体赋值
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
*	函 数 名: bsp_RelayOn
*	功能说明: 闭合继电器
*	形    参: uint8_t _no:继电器序号
*	返 回 值: void
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
*	函 数 名: bsp_RelayOff
*	功能说明: 断开继电器
*	形    参: uint8_t _no:继电器序号
*	返 回 值: void
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
*	函 数 名: bsp_RelayToggle
*	功能说明: 翻转指定继电器
*	形    参: uint8_t _no:继电器序号
*	返 回 值: void
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
*	函 数 名: bsp_IsRelayOn
*	功能说明: 判断继电器是否闭合
*	形    参: uint8_t _no:继电器序号
*	返 回 值: 1-闭合 0-断开
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

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
