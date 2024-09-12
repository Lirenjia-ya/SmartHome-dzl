/*
*********************************************************************************************************
*
*	模块名称 : 串口2驱动模块
*	文件名称 : bsp_uart2.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-20  Asuka    正式发布
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
uint8_t		USART2_RX_BUF[USART2_BUF_LEN]={0};	//接收缓存，最大USART_REC_LEN个字节，末字节为换行符
uint8_t		ReceiveState2=0;						//接收状态标记,1时接收完整一帧
uint16_t	RxCounter2=0;						//接收长度

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */

/*
*********************************************************************************************************
*	函 数 名: USART2_IRQHandler
*	功能说明: 串口2中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_IRQHandler(void) 
{ 
	uint8_t res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	
		res =USART_ReceiveData(USART2); //读取接收到的数据
		if(RxCounter2<USART2_BUF_LEN)
		{
			USART2_RX_BUF[RxCounter2]=res; //记录接收到的值
			RxCounter2++; //接收数据增加 1
		}
		//必须先读SR，再读DR，才能清除IDLE中断
		res=USART2->SR;//读SR寄存器
		res=USART2->DR;//读DR寄存器
		ReceiveState2=1;//标记接收状态置位，表示接受完1帧数据
	}   	
}  

/*
*********************************************************************************************************
*	函 数 名: Uart2_STA_Clr
*	功能说明: 串口2状态清除函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart2_STA_Clr(void)
{
	RxCounter2=0;
	ReceiveState2=0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart2
*	功能说明: 初始化CPU的UART2串口硬件设备。
*	形    参：uint32_t bound:波特率 
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart2(uint32_t bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//PA2 TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//PA3	RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
	
	
#ifdef EN_USART2_RX	//如果使能接收
	USART_InitStructure.USART_BaudRate=bound;//波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//收发模式
	
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;///抢占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 3;//响应优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断 
  
	USART_Cmd(USART2, ENABLE);                    //使能串口2
#endif
}

/*
*********************************************************************************************************
*	函 数 名: USART2_Send_Data
*	功能说明: USART2发送len个字节
*	形    参：buf:发送区首地址
*				len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_Send_Data(uint8_t *buf,uint8_t len) 
{ 
	uint8_t t;
	
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendData(USART2,buf[t]);
	}
	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);   
} 

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
