/*
*********************************************************************************************************
*
*	ģ������ : ����3����ģ��
*	�ļ����� : bsp_uart3.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-03  Asuka    ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

#ifdef EN_UART3_RX  //���ʹ���˽���

//���ջ�����
uint8_t UART3_RX_BUF[UART3_BUF_LEN];   //���ջ���,���UART3_BUF_LEN���ֽ�. 
//���յ������ݳ��� 
uint8_t UART3_RX_CNT=0;
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
*	�� �� ��: UART3_IRQHandler
*	����˵��:  ����3�ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_IRQHandler(void) 
{ 
	uint8_t res;      
  
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //���յ����� 
 {   
       
  res =USART_ReceiveData(USART3);     //��ȡ���յ������� 
  if(UART3_RX_CNT<UART3_BUF_LEN) 
  { 
   UART3_RX_BUF[UART3_RX_CNT]=res;  //��¼���յ���ֵ 
   UART3_RX_CNT++;     //������������1  
  }  
 }               
}  
#endif  
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart3
*	����˵��: ��ʼ��CPU��UART3����Ӳ���豸��
*	��    �Σ�uint32_t bound:������ 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart3(uint32_t bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;	
	USART_InitTypeDef		USART_InitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��UART3ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PB10 TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PB11	RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ
	
	
#ifdef EN_UART3_RX	//���ʹ�ܽ���
	USART_InitStructure.USART_BaudRate=bound;//����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;//��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(USART3,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;//ʹ�ܴ���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;///��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 3;//��Ӧ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж� 
  
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	

#endif

}
/*
*********************************************************************************************************
*	�� �� ��: UART3_Send_Data
*	����˵��: UART3����len���ֽ�
*	��    �Σ�buf:�������׵�ַ
*				len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART3_Send_Data(uint8_t *buf,uint8_t len) 
{ 
	uint8_t t; 
  
   for(t=0;t<len;t++)  //ѭ���������� 
	{      
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);    
		USART_SendData(USART3,buf[t]); 
	}   
  
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   
  
	UART3_RX_CNT=0;    
} 
/*
*********************************************************************************************************
*	�� �� ��: UART3_Receive_Data
*	����˵��:  UART3��ѯ���յ�������
*	��    �Σ�buf:���ջ����׵�ַ 
*   			len:���������ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART3_Receive_Data(uint8_t *buf,uint8_t *len) 
{ 
	uint8_t rxlen = UART3_RX_CNT; 
	uint8_t i=0; 
  
	*len=0;     //Ĭ��Ϊ0 
	delay_ms(10);    //�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս��� 
	
	if(rxlen==UART3_RX_CNT&&rxlen) //���յ�������,�ҽ�������� 
	{ 
		for(i=0;i<rxlen;i++) 
		{ 
			buf[i]=UART3_RX_BUF[i];
		}   
		*len=UART3_RX_CNT;  //��¼�������ݳ��� 
		UART3_RX_CNT=0;  //���� 
	} 
} 

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
