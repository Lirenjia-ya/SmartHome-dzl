/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_uart.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-20 waroyal  ��ʽ����
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
uint8_t		USART_RX_BUF[USART_REC_LEN]={0};	//���ջ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
uint8_t		ReceiveState=0;						//����״̬���,1ʱ��������һ֡
uint16_t	RxCounter=0;						//���ճ���

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ����1�ĳ�ʼ��
*	��    ��: uint32_t baud ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t baud)
{
	//1.����ṹ��
	//GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
	//USART
	USART_InitTypeDef	USART_InitStructure;
#if EN_USART1_RX
	//NVIC
	NVIC_InitTypeDef	NVIC_InitStructure;
#endif	
	
	//2.��ʱ��
	//��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//��USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//3.�˿ڳ�ʼ
	//TX,PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//RX,PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.���ڲ�������
	USART_InitStructure.USART_BaudRate=baud;											//������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//��Ӳ������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;							//�շ�ģʽ--ȫ˫��
	USART_InitStructure.USART_Parity=USART_Parity_No;									//����żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1;								//1λֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;							//���ݳ���8λ
	USART_Init(USART1,&USART_InitStructure);

#if EN_USART1_RX
	//5.�ж�����
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;			//����1�жϺ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;			//IRQʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;		//�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//�򿪽����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);			//�򿪿����ж�
#endif
	
	//6.����1ʹ��
	USART_Cmd(USART1,ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler
*	����˵��: USART1�жϣ���Ҫ�ǽ����жϺͿ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*	ע    �⣺��ȡUSART1->DR�ܱ���Ī������Ĵ���
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t Res=Res;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	//�����ж�
	{
		USART_RX_BUF[RxCounter++]=USART1->DR;			//��������ַ�
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)	//�����жϣ����յ�1֡���ݣ�
	{
		//�����ȶ�SR���ٶ�DR���������IDLE�ж�
		Res=USART1->SR;//��SR�Ĵ���
		Res=USART1->DR;//��DR�Ĵ���
		ReceiveState=1;//��ǽ���״̬��λ����ʾ������1֡����
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Uart_STA_Clr
*	����˵��: ����1״̬�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_STA_Clr(void)
{
	RxCounter=0;
	ReceiveState=0;
}

//��Ӧ��printf֧�ֺ���������
#if 1
#pragma import(__use_no_semihosting)
/*��׼����Ҫ��֧�ֺ���*/
struct __FILE
{
	int handle;
};

FILE __stdout;

/*����_sys_exit()�Ա���ʹ�ð�����ģʽ*/
void _sys_exit(int x)
{
	x=x;
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: 
*	�� �� ֵ: 
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
*	�� �� ��: USART1_Send_Data
*	����˵��: 
*	��    ��: 
*	�� �� ֵ: 
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

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
