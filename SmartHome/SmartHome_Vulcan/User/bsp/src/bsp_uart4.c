/*
*********************************************************************************************************
*
*	ģ������ : xx����ģ��
*	�ļ����� : bsp_uart4.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    20xx-xx-xx  waroyal  ��ʽ����
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
uint8_t		USART4_RX_BUF[UART4_BUF_LEN]={0};	//���ջ��棬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
uint8_t		ReceiveState4=0;						//����״̬���,1ʱ��������һ֡
uint16_t	RxCounter4=0;						//���ճ���

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
* �� �� ��: UART4_IRQHandler
* ����˵��: ���� 4 �жϡ�
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
	uint8_t res;
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //���յ�����
	{	
		res =USART_ReceiveData(UART4); //��ȡ���յ�������
		if(RxCounter4<UART4_BUF_LEN)
		{
			USART4_RX_BUF[RxCounter4]=res; //��¼���յ���ֵ
			RxCounter4++; //������������ 1
		}
		ReceiveState4=1;//��ǽ���״̬��λ����ʾ������1֡����
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Uart4_STA_Clr
*	����˵��: ����4״̬�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart4_STA_Clr(void)
{
	RxCounter4=0;
	ReceiveState4=0;
}


/*
*********************************************************************************************************
* �� �� ��: bsp_InitUart4
* ����˵��: ��ʼ�� CPU �� UART4 ����Ӳ���豸��
* �� �Σ�uint32_t bound:������
* �� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_InitUart4(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ�� GPIOC ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ�� UART4 ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,ENABLE);//��λ���� 4
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4,DISABLE);//ֹͣ��λ
	
#ifdef EN_UART4_RX //���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8 λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); ; //��ʼ������
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //ʹ�ܴ��� 4 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
	
	USART_Cmd(UART4, ENABLE); //ʹ�ܴ���
#endif
}

/*
*********************************************************************************************************
* �� �� ��: UART4_Send_Data
* ����˵��: UART4 ���� len ���ֽ�
* �� �Σ� buf:�������׵�ַ
* len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� 64 ���ֽ�)
* �� �� ֵ: ��
*********************************************************************************************************
*/
void USART4_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	
	for(t=0;t<len;t++) //ѭ����������
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
		USART_SendData(UART4,buf[t]);
	}
	
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
