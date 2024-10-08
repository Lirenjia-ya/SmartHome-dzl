/*
*********************************************************************************************************
*
*	模块名称 : 风扇驱动模块
*	文件名称 : bsp_Fan.c
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-05-29
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_FAN_H_
#define _BSP_FAN_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

//////////////////////////////////////////////////
/* 宏定义区域 */
//移植修改区
//按照实际端口修改
//较前版本更加容易移植（注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTSG引脚，需要打开AFIO时钟，并失能JTAG）

#ifdef WSNEP_V01

#define RCC_ALL_FAN	(FAN1_GPIO_CLK|FAN2_GPIO_CLK)

#define FAN1_GPIO_PIN		GPIO_Pin_8				//FAN1引脚号
#define FAN1_PIN_ID			8						//FAN1引脚序号
#define FAN1_GPIO_PORT		GPIOB					//FAN1端口号
#define FAN1_GPIO_CLK		RCC_APB2Periph_GPIOB	//FAN1时钟
#define FAN1_FUN_OUT		PBout					//FAN1输出端口配置函数
//#define FAN1_FUN_IN		PBin					//FAN1输入端口配置函数

#define FAN2_GPIO_PIN		GPIO_Pin_9				//FAN2引脚号
#define FAN2_PIN_ID			9						//FAN2引脚序号
#define FAN2_GPIO_PORT		GPIOB					//FAN2端口号
#define FAN2_GPIO_CLK		RCC_APB2Periph_GPIOB	//FAN2时钟
#define FAN2_FUN_OUT		PBout					//FAN2输出端口配置函数
//#define RELAY2_FUN_IN		PBin					//FAN2输入端口配置函数

#endif

//IO操作函数
#define FAN1	FAN1_FUN_OUT(FAN1_PIN_ID)
#define FAN2	FAN2_FUN_OUT(FAN2_PIN_ID)
//////////////////////////////////////////////////////

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitFan(void);
void bsp_FanOn(uint8_t _no);
void bsp_FanOff(uint8_t _no);
void bsp_FanToggle(uint8_t _no);
uint8_t bsp_IsFanOn(uint8_t _no);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

