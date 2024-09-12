/*
*********************************************************************************************************
*
*	ģ������ : ����2����ģ��
*	�ļ����� : bsp_uart2.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-20  Asuka    ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

#ifdef EN_USART2_RX  //���ʹ���˽���

//���ջ�����
uint8_t USART2_RX_BUF[USART2_BUF_LEN];   //���ջ���,���USART3_BUF_LEN���ֽ�. 
//���յ������ݳ��� 
uint8_t USART2_RX_CNT=0;  
///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */

/*
*********************************************************************************************************
*	�� �� ��: USART2_IRQHandler
*	����˵��: ����2�ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_IRQHandler(void) 
{ 
	uint8_t res;      
  
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //���յ����� 
 {   
       
  res =USART_ReceiveData(USART2);     //��ȡ���յ������� 
  if(USART2_RX_CNT<USART2_BUF_LEN) 
  { 
   USART2_RX_BUF[USART2_RX_CNT]=res;  //��¼���յ���ֵ 
   USART2_RX_CNT++;     //������������1  
  }  
 }               
}  
#endif  
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart2
*	����˵��: ��ʼ��CPU��UART2����Ӳ���豸��
*	��    �Σ�uint32_t bound:������ 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart2(uint32_t bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//PA2 TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//PA3	RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
	
	
#ifdef EN_USART2_RX	//���ʹ�ܽ���
	USART_InitStructure.USART_BaudRate=bound;//����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;//��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;///��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 3;//��Ӧ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж� 
  
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2
#endif
}
/*
*********************************************************************************************************
*	�� �� ��: USART2_Send_Data
*	����˵��: USART2����len���ֽ�
*	��    �Σ�buf:�������׵�ַ
*				len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Send_Data(uint8_t *buf,uint8_t len) 
{ 
	uint8_t t; 
  
   for(t=0;t<len;t++)  //ѭ���������� 
	{      
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);    
		USART_SendData(USART2,buf[t]); 
	}   
  
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   
  
	USART2_RX_CNT=0;    
} 
/*
*********************************************************************************************************
*	�� �� ��: UART2_Receive_Data
*	����˵��: UART2��ѯ���յ�������
*	��    �Σ�buf:���ջ����׵�ַ 
*   				len:���������ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Receive_Data(uint8_t *buf,uint8_t *len) 
{ 
	uint8_t rxlen = USART2_RX_CNT; 
	uint8_t i=0; 
  
	*len=0;     //Ĭ��Ϊ0 
	delay_ms(10);    //�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս��� 
	//printf("������\n");
	if(rxlen==USART2_RX_CNT&&rxlen) //���յ�������,�ҽ�������� 
	{ 
		for(i=0;i<rxlen;i++) 
		{ 
			buf[i]=USART2_RX_BUF[i];  
		} 
		//printf("���ճɹ�\n");
		*len=USART2_RX_CNT;  //��¼�������ݳ��� 
		USART2_RX_CNT=0;  //���� 
 } 
} 

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
