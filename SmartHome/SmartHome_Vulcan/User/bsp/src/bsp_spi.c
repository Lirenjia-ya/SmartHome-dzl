/*
*********************************************************************************************************
*
*	模块名称 : SPI驱动模块
*	文件名称 : bsp_spi.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-27 waroyal  正式发布
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
* 函 数 名: SPI_RCC_Configuration
* 功能说明: 使能 SPI 时钟
* 形 参: SPIx 需要使用的 SPI
* 返 回 值: 无
*********************************************************************************************************
*/
static void SPI_RCC_Configuration(SPI_TypeDef* SPIx)
{
	if(SPIx==SPI1)
	{
		RCC_SPI1(SPI1_CLK,ENABLE); //使能 SPI1 时钟
	}
	else
	{
		RCC_SPI2(SPI2_CLK,ENABLE); //使能 SPI2 时钟
	}
}

/*
*********************************************************************************************************
* 函 数 名: SPI_GPIO_Configuration
* 功能说明: 配置指定 SPI 的引脚
* 形 参: SPIx 需要使用的 SPI
* 返 回 值: 无
*********************************************************************************************************
*/
static void SPI_GPIO_Configuration(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef SPI_InitStructure;
	 if(SPIx==SPI1)
	{
		RCC_APB2PeriphClockCmd(SPI1_GPIO_ALL_CLK,ENABLE); //使能 SPI1 端口时钟
		SPI_InitStructure.GPIO_Pin = SPI1_SCK_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI1_SCK_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI1_SCK_GPIO_PORT,SPI1_SCK_GPIO_PIN); //上拉
		
		SPI_InitStructure.GPIO_Pin = SPI1_MISO_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI1_MISO_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI1_MISO_GPIO_PORT,SPI1_MISO_GPIO_PIN); //上拉
		
		SPI_InitStructure.GPIO_Pin = SPI1_MOSI_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI1_MOSI_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI1_MOSI_GPIO_PORT,SPI1_MOSI_GPIO_PIN); //上拉
	}
	else
	{
		RCC_APB2PeriphClockCmd(SPI2_GPIO_ALL_CLK,ENABLE); //使能 SPI2 端口时钟
		SPI_InitStructure.GPIO_Pin = SPI2_SCK_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI2_SCK_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI2_SCK_GPIO_PORT,SPI2_SCK_GPIO_PIN); //上拉
		
		SPI_InitStructure.GPIO_Pin = SPI2_MISO_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI2_MISO_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI2_MISO_GPIO_PORT,SPI2_MISO_GPIO_PIN); //上拉
		
		SPI_InitStructure.GPIO_Pin = SPI2_MOSI_GPIO_PIN;
		SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		SPI_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_Init(SPI2_MOSI_GPIO_PORT, &SPI_InitStructure);
		GPIO_SetBits(SPI2_MOSI_GPIO_PORT,SPI2_MOSI_GPIO_PIN); //上拉
	} 
}

/*
*********************************************************************************************************
* 函 数 名: SPIx_SetCPOL_CPHA
* 功能说明: 配置指定 SPI CPOL 极性 CPHA 采样时钟的位置
* 形 参: SPIx 需要使用的 SPI
* CPOL Set:SPI_CPOL_Low,SPI_CPOL_High
* CPHA Set:SPI_CPHA_1Edge,SPI_CPHA_2Edge
* 返 回 值: 无
* 注 意：必须在 bsp_InitSPIx 函数后调用
*********************************************************************************************************
*/
void SPIx_SetCPOL_CPHA(SPI_TypeDef* SPIx,uint16_t SPI_CPOL,uint16_t SPI_CPHA)
{
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_CPOL(SPI_CPOL));
	assert_param(IS_SPI_CPHA(SPI_CPHA));
	//设置 SPIx 串行同步时钟的空闲状态为?电平
	if(SPI_CPOL == SPI_CPOL_High)
	{
		SPIx->CR1 |= SPI_CPOL_High;
	}
	else
	{
		SPIx->CR1 &= (uint16_t)~SPI_CPOL_High;
	}
	//设置 SPIx 串行同步时钟的第?个跳变沿（上升或下降）数据被采样
	if(SPI_CPHA == SPI_CPHA_2Edge )
	{
		SPIx->CR1 |= SPI_CPHA_2Edge;
	}
	else
	{
		SPIx->CR1 &= (uint16_t)~SPI_CPHA_2Edge;
	}
	SPI_Cmd(SPIx,ENABLE); 
}

/*
*********************************************************************************************************
* 函 数 名: SPIx_SetSpeed
* 功能说明: SPI 速度设置函数
* 形 参: SPIx 需要使用的 SPI
* SpeedSet:
* SPI_BaudRatePrescaler_2 2 分频 
* SPI_BaudRatePrescaler_8 8 分频 
* SPI_BaudRatePrescaler_16 16 分频 
* SPI_BaudRatePrescaler_256 256 分频
* 返 回 值: 无
* 注 意：必须在 bsp_InitSPIx 函数后调用
*********************************************************************************************************
*/
void SPIx_SetSpeed(SPI_TypeDef* SPIx,uint16_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIx->CR1 &= (uint16_t)~SPI_BaudRatePrescaler_256; //只清除速度设置位
	SPIx->CR1 |= SPI_BaudRatePrescaler; //设置 SPIx 速度
	SPI_Cmd(SPIx,ENABLE); 
}

/*
*********************************************************************************************************
* 函 数 名: bsp_InitSPIx
* 功能说明: 配置 SPIx 相关的内容。
* 形 参: SPIx 需要使用的 SPI
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIx(SPI_TypeDef* SPIx)
{ 
	SPI_InitTypeDef SPI_InitStructure;
	SPI_RCC_Configuration(SPIx);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置 SPI 单向或者双向的数据模式:SPI 设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置 SPI 工作模式:设置为主 SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //设置 SPI 的数据大小:SPI 发送接收 8 位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //串行同步时钟的空闲状态为高电平
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS 信号由硬件（NSS 管脚）还是软件（使用 SSI 位）管理:内部 NSS 信号有 SSI 位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //指定数据传输从 MSB 位还是LSB 位开始:数据传输从 MSB 位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC 值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure); //根据 SPI_InitStruct 中指定的参数初始化外设 SPIx 寄存器
	SPI_GPIO_Configuration(SPIx); //端口配置
	SPI_Cmd(SPIx, ENABLE); //使能SPIx
}

/*
*********************************************************************************************************
* 函 数 名: SPI_WriteByte
* 功能说明: 写 1 字节数据到 SPI 总线
* 形 参: TxData 写到总线的数据
* 返 回 值: 数据发送状态:0 数据发送成功,-1 数据发送失败
*********************************************************************************************************
*/
int32_t SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t TxData)
{
	uint8_t retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的 SPI 标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return -1;
	} 
	SPI_I2S_SendData(SPIx, TxData); //通过外设 SPIx 发送一个数据
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的 SPI 标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return -1;
	} 
	return 0; //返回正确发送
}		

/*
*********************************************************************************************************
* 函 数 名: SPI_ReadByte
* 功能说明: 从 SPI 总线读取 1 字节数据
* 形 参: p_RxData 数据储存地址
* 返 回 值: 数据读取状态:0 数据发送成功,-1 数据发送失败
*********************************************************************************************************
*/
int32_t SPI_ReadByte(SPI_TypeDef* SPIx, uint16_t *p_RxData)
{
	uint8_t retry=0;
	//while((SPIx->SR&SPI_I2S_FLAG_TXE)==0) //等待发送区空
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的 SPI 标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return -1;
	} 
	SPI_I2S_SendData(SPIx, 0xFF); //通过外设 SPIx 发送一个 dumy 数据
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的 SPI 标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return -1;
	}
	*p_RxData = SPI_I2S_ReceiveData(SPIx); 
	return 0; //返回正确接收
}

/*
*********************************************************************************************************
* 函 数 名: SPI_WriteNBytes
* 功能说明: 向 SPI 总线写多字节数据
* 形 参: p_TxData 发送数据缓冲区首地址
* sendDataNum 发送数据字节数
* 返 回 值: 数据发送状态:0 数据发送成功,-1 数据发送失败
*********************************************************************************************************
*/
int32_t SPI_WriteNBytes(SPI_TypeDef* SPIx, uint8_t *p_TxData,uint32_t sendDataNum)
{
	uint16_t retry=0;
	while(sendDataNum--)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的 SPI 标志位设置与否:发送缓存空标志位
		{
			retry++;
			if(retry>20000)return -1;
		}
		SPI_I2S_SendData(SPIx, *p_TxData++); //通过外设 SPIx 发送一个数据
		retry=0; 
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的 SPI 标志位设置与否:接受缓存非空标志位
		{
			retry++;
			if(retry>20000)return -1;
		}
	}
	return 0;//返回正确发送
}

/*
*********************************************************************************************************
* 函 数 名: SPI_ReadNBytes
* 功能说明: 从 SPI 总线读取多字节数据
* 形 参: p_RxData 数据储存地址
* readDataNum 读取数据字节数
* 返 回 值: 数据读取状态:0 数据发送成功,-1 数据发送失败
*********************************************************************************************************
*/
int32_t SPI_ReadNBytes(SPI_TypeDef* SPIx, uint8_t *p_RxData,uint32_t readDataNum)
{
	uint16_t retry=0;
	while(readDataNum--)
	{
		SPI_I2S_SendData(SPIx, 0xFF); //通过外设 SPIx 发送一个 dumy 数据
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的 SPI 标志位设置与否:发送缓存空标志位
		{
			retry++;
			if(retry>20000)return -1;
		}
		retry = 0;
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的 SPI 标志位设置与否:接受缓存非空标志位
		{
			retry++;
			if(retry>20000)return -1;
		}
		*p_RxData++ = SPI_I2S_ReceiveData(SPIx);
	}
	return 0;//返回正确接收
}

/*
*********************************************************************************************************
* 函 数 名: SPIx_ReadWriteByte
* 功能说明: 从 SPI 总线读写一个字节
* 形 参: SPIx 需要使用的 SPI
* TxData:要写入的字节
* 返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t SPIx_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData)
{
	u8 retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的 SPI 标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPIx, TxData); //通过外设 SPIx 发送一个数据
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的 SPI 标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	} 
	return SPI_I2S_ReceiveData(SPIx); //返回通过 SPIx 最近接收的数据 
}

/*
*********************************************************************************************************
* 函 数 名: SPI1_ReadWriteByte
* 功能说明: 从 SPI 总线读写一个字节
* 形 参: TxData:要写入的字节
* 返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
	uint8_t temp;
	temp = SPIx_ReadWriteByte(SPI1,TxData);
	return temp;
}

/*
*********************************************************************************************************
* 函 数 名: SPI2_ReadWriteByte
* 功能说明: 从 SPI 总线读写一个字节
* 形 参: TxData:要写入的字节
* 返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
	uint8_t temp;
	temp = SPIx_ReadWriteByte(SPI2,TxData);
	return temp;
}
///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
