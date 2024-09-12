/*
*********************************************************************************************************
*
*	模块名称 : 串口驱动模块
*	文件名称 : bsp_uart.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-20 waroyal  正式发布
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
uint8_t		USART_RX_BUF[USART_REC_LEN]={0};	//接收缓存，最大USART_REC_LEN个字节，末字节为换行符
uint8_t		ReceiveState=0;						//接收状态标记,1时接收完整一帧
uint16_t	RxCounter=0;						//接收长度

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 串口1的初始化
*	形    参: uint32_t baud 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t baud)
{
	//1.定义结构体
	//GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
	//USART
	USART_InitTypeDef	USART_InitStructure;
#if EN_USART1_RX
	//NVIC
	NVIC_InitTypeDef	NVIC_InitStructure;
#endif	
	
	//2.打开时钟
	//打开GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//打开USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//3.端口初始
	//TX,PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//RX,PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.串口参数设置
	USART_InitStructure.USART_BaudRate=baud;											//波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//无硬件流控
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;							//收发模式--全双工
	USART_InitStructure.USART_Parity=USART_Parity_No;									//无奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;								//1位停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;							//数据长度8位
	USART_Init(USART1,&USART_InitStructure);

#if EN_USART1_RX
	//5.中断设置
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;			//串口1中断号
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;			//IRQ使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;		//子优先级3
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//打开接收中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);			//打开空闲中断
#endif
	
	//6.串口1使能
	USART_Cmd(USART1,ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: USART1中断，主要是接收中断和空闲中断
*	形    参: 无
*	返 回 值: 无
*	注    意：读取USART1->DR能避免莫名其妙的错误
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t Res=Res;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	//接收中断
	{
		USART_RX_BUF[RxCounter++]=USART1->DR;			//保存接收字符
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)	//接收中断（接收到1帧数据）
	{
		//必须先读SR，再读DR，才能清除IDLE中断
		Res=USART1->SR;//读SR寄存器
		Res=USART1->DR;//读DR寄存器
		ReceiveState=1;//标记接收状态置位，表示接受完1帧数据
	}
}

/*
*********************************************************************************************************
*	函 数 名: Uart_STA_Clr
*	功能说明: 串口1状态清除函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_STA_Clr(void)
{
	RxCounter=0;
	ReceiveState=0;
}

//对应的printf支持函数或设置
#if 1
#pragma import(__use_no_semihosting)
/*标准库需要的支持函数*/
struct __FILE
{
	int handle;
};

FILE __stdout;

/*定义_sys_exit()以避免使用半主机模式*/
void _sys_exit(int x)
{
	x=x;
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
int fputc(int ch,FILE *f)
{
	while((USART1->SR&0X40)==0){};	//
	USART1->DR=(u8)ch;
	return ch;
}
#endif
//--------------------------------

/*
*********************************************************************************************************
*	函 数 名: USART1_Send_Data
*	功能说明: 
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USART1_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		USART_SendData(USART1,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}
	

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
