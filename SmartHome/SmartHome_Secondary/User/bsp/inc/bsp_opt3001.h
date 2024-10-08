/*
*********************************************************************************************************
*
*	模块名称 : opt3001模块
*	文件名称 : bsp_opt3001.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef __BSP_OPT3001_H
#define __BSP_OPT3001_H

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

///////////////////////////////////////
/* 宏定义区域 */

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
//ADD 引脚链接到 VCC
#define OPT3001_ADDR 					0x8A


/********************** CMD 命令开始 **********************/
#define OPT3001_MANUFACTURER_ID 		0x7E
#define OPT3001_DEVICE_ID 				0x7F
#define OPT3001_RESULT					0x00
#define OPT3001_CONFIGURATION 			0x01
/********************** CMD 命令结束 **********************/

#define OPT3001_ManufacturerID 			0x5449
#define OPT3001_DeviceID 				0x3001



uint8_t bsp_InitOpt3001(void); //初始化 IIC
void OPT3001Config(void);
uint32_t GetLuxValue(void);




///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

