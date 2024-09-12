/*
*********************************************************************************************************
*
*	模块名称 : SPI模块
*	文件名称 : bsp_spi.h
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-27
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"
///////////////////////////////////////
/* 宏定义区域 */

/****************************************************************************/
#define  RCC_SPI1     RCC_APB2PeriphClockCmd
#define  SPI1_CLK     RCC_APB2Periph_SPI1
#define  SPI1_GPIO_ALL_CLK      ( SPI1_SCK_GPIO_CLK | SPI1_MISO_GPIO_CLK | SPI1_MOSI_GPIO_CLK )
#define  SPI1_SCK_GPIO_PIN      GPIO_Pin_5 //SPI1_SCK 引脚号
#define  SPI1_SCK_GPIO_PORT     GPIOA //SPI1_SCK 端口号
#define  SPI1_SCK_GPIO_CLK      RCC_APB2Periph_GPIOA//SPI1_SCK 时钟
#define  SPI1_MISO_GPIO_PIN     GPIO_Pin_6 //SPI1_MISO 引脚号
#define  SPI1_MISO_GPIO_PORT    GPIOA //SPI1_MISO 端口号
#define  SPI1_MISO_GPIO_CLK     RCC_APB2Periph_GPIOA//SPI1_MISO 时钟
#define  SPI1_MOSI_GPIO_PIN     GPIO_Pin_7 //SPI1_MOSI 引脚号
#define  SPI1_MOSI_GPIO_PORT    GPIOA //SPI1_MOSI 端口号
#define  SPI1_MOSI_GPIO_CLK     RCC_APB2Periph_GPIOA//SPI1_MOSI 时钟

/****************************************************************************/
#define  RCC_SPI2    RCC_APB1PeriphClockCmd
#define  SPI2_CLK    RCC_APB1Periph_SPI2
#define  SPI2_GPIO_ALL_CLK 			( SPI2_SCK_GPIO_CLK | SPI2_MISO_GPIO_CLK | SPI2_MOSI_GPIO_CLK )
#define  SPI2_SCK_GPIO_PIN 			GPIO_Pin_13 //SPI2_SCK 引脚号
#define  SPI2_SCK_GPIO_PORT		  GPIOB //SPI2_SCK 端口号
#define  SPI2_SCK_GPIO_CLK 			RCC_APB2Periph_GPIOB//SPI2_SCK 时钟
#define  SPI2_MISO_GPIO_PIN 		GPIO_Pin_14 //SPI2_MISO 引脚号
#define  SPI2_MISO_GPIO_PORT 		GPIOB //SPI2_MISO 端口号
#define  SPI2_MISO_GPIO_CLK 		RCC_APB2Periph_GPIOB//SPI2_MISO 时钟
#define  SPI2_MOSI_GPIO_PIN 		GPIO_Pin_15 //SPI2_MOSI 引脚号
#define  SPI2_MOSI_GPIO_PORT	  GPIOB //SPI2_MOSI 端口号
#define  SPI2_MOSI_GPIO_CLK     RCC_APB2Periph_GPIOB//SPI2_MOSI 时钟
///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
//SPI 所有操作函数
void bsp_InitSPIx(SPI_TypeDef* SPIx); //初始化 SPIx
void SPIx_SetCPOL_CPHA(SPI_TypeDef* SPIx,uint16_t SPI_CPOL,uint16_t SPI_CPHA);
void SPIx_SetSpeed(SPI_TypeDef* SPIx,uint16_t SPI_BaudRatePrescaler);
int32_t SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t TxData);
int32_t SPI_ReadByte(SPI_TypeDef* SPIx, uint16_t *p_RxData);
int32_t SPI_WriteNBytes(SPI_TypeDef* SPIx, uint8_t *p_TxData,uint32_t sendDataNum);
int32_t SPI_ReadNBytes(SPI_TypeDef* SPIx, uint8_t *p_RxData,uint32_t readDataNum);
uint8_t SPIx_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);
uint8_t SPI2_ReadWriteByte(uint8_t TxData);
///////////////////////////////////////
#endif

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/

