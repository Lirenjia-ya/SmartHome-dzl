/*
*********************************************************************************************************
*
*	模块名称 : 模块
*	文件名称 : .h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-08
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

///////////////////////////////////////
/* 宏定义区域 */
#define RCC_ALL_KEY 	(WKUP_GPIO_CLK|KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK)
//WKUP--------------------------------------------------------------------
#define WKUP_GPIO_PIN		GPIO_Pin_0				//WKUP引脚号
#define WKUP_PIN_ID			0						//WKUP引脚序号
#define WKUP_GPIO_PORT		GPIOA					//WKUP端口号
#define WKUP_GPIO_CLK		RCC_APB2Periph_GPIOA	//WKUP时钟
//#define WKUP_FUN_OUT		PAout					//WKUP输出端口配置函数
#define WKUP_FUN_IN			PAin					//WKUP输入端口配置函数
#define WKUP_GPIO_MODE		GPIO_Mode_IPD			//WKUP输入模式
#define WKUP_GPIO_LEVEL		1						//WKUP的有效电平

//KEY0--------------------------------------------------------------------
#define KEY0_GPIO_PIN		GPIO_Pin_4				//KEY0引脚号
#define KEY0_PIN_ID			4						//KEY0引脚序号
#define KEY0_GPIO_PORT		GPIOE					//KEY0端口号
#define KEY0_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY0时钟
//#define WKUP_FUN_OUT		PEout					//KEY0输出端口配置函数
#define KEY0_FUN_IN			PEin					//KEY0输入端口配置函数
#define KEY0_GPIO_MODE		GPIO_Mode_IPU			//KEY0输入模式
#define KEY0_GPIO_LEVEL		0						//KEY0的有效电平

//KEY1--------------------------------------------------------------------
#define KEY1_GPIO_PIN		GPIO_Pin_3				//KEY1引脚号
#define KEY1_PIN_ID			3						//KEY1引脚序号
#define KEY1_GPIO_PORT		GPIOE					//KEY1端口号
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY1时钟
//#define WKUP_FUN_OUT		PEout					//KEY1输出端口配置函数
#define KEY1_FUN_IN			PEin					//KEY1输入端口配置函数
#define KEY1_GPIO_MODE		GPIO_Mode_IPU			//KEY1输入模式
#define KEY1_GPIO_LEVEL		0						//KEY1的有效电平

//KEY2--------------------------------------------------------------------
#define KEY2_GPIO_PIN		GPIO_Pin_2				//KEY2引脚号
#define KEY2_PIN_ID			2						//KEY2引脚序号
#define KEY2_GPIO_PORT		GPIOE					//KEY2端口号
#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY2时钟
//#define WKUP_FUN_OUT		PEout					//KEY2输出端口配置函数
#define KEY2_FUN_IN			PEin					//KEY2输入端口配置函数
#define KEY2_GPIO_MODE		GPIO_Mode_IPU			//KEY2输入模式
#define KEY2_GPIO_LEVEL		0						//KEY2的有效电平

//IO操作函数	 
#define WKUP    WKUP_FUN_IN(WKUP_PIN_ID) //WKUP
#define KEY0    KEY0_FUN_IN(KEY0_PIN_ID) //KEY0
#define KEY1    KEY1_FUN_IN(KEY1_PIN_ID) //KEY1
#define KEY2    KEY2_FUN_IN(KEY2_PIN_ID) //KEY2


/*简单按键返回值-----------------------------------------------------------------------------------------------------------------------------------------------
#define NOKEY_NUM	0
#define WKUP_NUM	1	//WKUP
#define KEY0_NUM	2	//KEY0
#define KEY1_NUM	3	//KEY1
#define KEY2_NUM	4	//KEY2
-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define KEY_COUNT 6	//按键个数，4个独立按键+2个组合键

/*根据应用程序的功能重命名按键宏*/
#define WKUP_DOWN 	KEY_1_DOWN
#define WKUP_UP 	KEY_1_UP
#define WKUP_LONG 	KEY_1_LONG

#define KEY0_DOWN 	KEY_2_DOWN
#define KEY0_UP 	KEY_2_UP
#define KEY0_LONG 	KEY_2_LONG

#define KEY1_DOWN 	KEY_3_DOWN
#define KEY1_UP 	KEY_3_UP
#define KEY1_LONG 	KEY_3_LONG

#define KEY2_DOWN 	KEY_4_DOWN
#define KEY2_UP 	KEY_4_UP
#define KEY2_LONG 	KEY_4_LONG

#define SYS_DOWN_WKUP_KEY0 	KEY_5_DOWN	//组合键WKUP+KEY0
#define SYS_UP_WKUP_KEY0 	KEY_5_UP
#define SYS_LONG_WKUP_KEY0 	KEY_5_LONG

#define SYS_DOWN_KEY0_KEY1 	KEY_6_DOWN	//组合键KEY0+KEY1
#define SYS_UP_KEY0_KEY1	KEY_6_UP
#define SYS_LONG_KEY0_KEY1 	KEY_6_LONG


/*按键id，主要用于bsp_KeyState()函数入口参数*/
typedef enum
{
	KID_K1=0,
	KID_K2,
	KID_K3,
	KID_K4,
}KEY_ID_E;


/*
按键滤波时间50ms，单位10ms
只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
*/
#define KEY_FILTER_TIME 5
#define KEY_LONG_TIME 	100	//	单位10ms，持续1s，认为长按事件



/*定义键值代码*/
typedef enum
{
	KEY_NONE=0,	//0表示按键事件
	
	KEY_1_DOWN,	//按下
	KEY_1_UP,	//弹起
	KEY_1_LONG,	//长按
	
	KEY_2_DOWN,
	KEY_2_UP,
	KEY_2_LONG,
	
	KEY_3_DOWN,
	KEY_3_UP,
	KEY_3_LONG,
	
	KEY_4_DOWN,
	KEY_4_UP,
	KEY_4_LONG,
	
	//组合键
	KEY_5_DOWN,
	KEY_5_UP,
	KEY_5_LONG,
	
	KEY_6_DOWN,
	KEY_6_UP,
	KEY_6_LONG,
}KEY_ENUM;


/*按键结构体成员*/
typedef struct
{
	uint8_t (*IsKeyDownFunc)(void);	//按键按下的判断函数，1表示按下
	
	uint8_t Count;					//滤波器计数器？
	uint16_t LongCount;				//长按计数器
	uint16_t LongTime;				//按键按下持续时间，初始化0时表示不检测长按
	uint8_t State;					//按键当前状态（按下or弹起）
	uint8_t RepeatSpeed;			//连续按键周期
	uint8_t RepeatCount;			//连续按键计数器
}KEY_T;

/*按键FIFO用到的变量*/
#define KEY_FIFO_SIZE 10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];	//键值缓冲区
	uint8_t Read;				//缓冲区读指针1
	uint8_t Write;				//缓冲区写指针
	uint8_t Read2;				//缓冲区读指针2
}KEY_FIFO_T;



///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
void bsp_InitKey(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID,uint16_t _LongTime,uint8_t _RepeatSpeed);
void bsp_ClearKey(void);
void bsp_KeyScan(void);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

