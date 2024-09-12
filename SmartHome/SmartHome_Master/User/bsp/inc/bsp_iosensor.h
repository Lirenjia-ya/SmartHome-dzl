/*
*********************************************************************************************************
*
*	模块名称 : 传感器模块
*	文件名称 : bsp_iosensor.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-04-12
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_IOSENSOR_H_
#define _BSP_IOSENSOR_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

////////////////////////
//移植修改区
//按照实际端口修改
//较前版本更加容易移植（注意，端口仅仅适用于非JTAG/SWD引脚，如果是JTSG引脚，需要打开AFIO时钟，并失能JTAG）


////////////////////////

///////////////////////////////////////
/* 宏定义区域 */
//DO101 震动传感器
#define VIBRATE_OUT_GPIO_PIN		GPIO_Pin_11					//VIBRATE_OUT引脚号
#define VIBRATE_OUT_PIN_ID			11							//VIBRATE_OUT引脚序号
#define VIBRATE_OUT_GPIO_PORT		GPIOA						//VIBRATE_OUT端口号
#define VIBRATE_OUT_GPIO_CLK		RCC_APB2Periph_GPIOA		//VIBRATE_OUT时钟
//#define VIBRATE_OUT_FUN_OUT			PAout						//VIBRATE_OUT输出端口配置函数
#define VIBRATE_OUT_FUN_IN			PAin						//VIBRATE_OUT输入端口配置函数
#define VIBRATE_OUT_GPIO_MODE		GPIO_Mode_IPU				//VIBRATE_OUT外部中断输入函数
#define VIBRATE_OUT_ACTIVE_LEVEL	0							//VIBRATE_OUT有效电平为低电平

#define VIBRATE_OUT_PORT_SRC		GPIO_PortSourceGPIOA		//VIBRATE_OUT外部中断端口号
#define VIBRATE_OUT_PIN_SRC			GPIO_PinSource11			//VIBRATE_OUT外部中断引脚号
#define VIBRATE_OUT_EXTI_LINE		EXTI_Line11					//VIBRATE_OUT外部中断号
#define VIBRATE_OUT_EXTI_TRIG		EXTI_Trigger_Falling		//VIBRATE_OUT外部中断触发方式
#define VIBRATE_OUT_EXTI_IRQN		EXTI15_10_IRQn				//VIBRATE_OUT外部中断NVIC号

////////////////////////
//PIR 人体红外传感器
#define PIR_OUT_GPIO_PIN			GPIO_Pin_3					//PIR_OUT引脚号
#define PIR_OUT_PIN_ID				3							//PIR_OUT引脚序号
#define PIR_OUT_GPIO_PORT			GPIOB						//PIR_OUT端口号
#define PIR_OUT_GPIO_CLK			RCC_APB2Periph_GPIOB		//PIR_OUT时钟
//#define PIR_OUT_FUN_OUT				PBout						//PIR_OUT输出端口配置函数
#define PIR_OUT_FUN_IN				PBin						//PIR_OUT输入端口配置函数
#define PIR_OUT_GPIO_MODE			GPIO_Mode_IPD				//PIR_OUT外部中断输入函数
#define PIR_OUT_ACTIVE_LEVEL		1							//PIR_OUT有效电平为高电平

#define PIR_OUT_PORT_SRC			GPIO_PortSourceGPIOB		//PIR_OUT外部中断端口号
#define PIR_OUT_PIN_SRC				GPIO_PinSource3			//PIR_OUT外部中断引脚号
#define PIR_OUT_EXTI_LINE			EXTI_Line3					//PIR_OUT外部中断号
#define PIR_OUT_EXTI_TRIG			EXTI_Trigger_Rising			//PIR_OUT外部中断触发方式
#define PIR_OUT_EXTI_IRQN			EXTI15_10_IRQn				//PIR_OUT外部中断NVIC号

////////////////////////
//IO操作函数
#define VIBRATE_OUT			VIBRATE_OUT_FUN_IN(VIBRATE_OUT_PIN_ID)		//读取VIBRATE_OUT 低电平有效
#define PIR_OUT				PIR_OUT_FUN_IN(PIR_OUT_PIN_ID)				//读取PIR_OUT 高电平有效

///////////////////////////////////////
/* 外部变量申明区 */
extern uint8_t VIBRATE_OUT_flag;
extern uint8_t PIR_OUT_flag;

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitIOSensor(void);

///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

