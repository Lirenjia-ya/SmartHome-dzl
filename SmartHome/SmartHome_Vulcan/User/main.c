/*
*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : �������ӡ�
*	�޸ļ�¼ :
*		�汾��  ����       ����     ˵��
*		V1.0    2024-03-06 waroyal  �׷�
*
*	Copyright (C), 2014-2019, �������׵���
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */

/* ���� LCD ��ʾ��ع�����Ϣ */
#define DEV_NAME 				"SmartHome"
#define TEMPERATURE 		"temp:"
#define HUMIDITY			 	"humi:"
#define CO2							"co2 :"
#define PM2_5						"pm2.5:"
#define LIGHT						"light:"

//���� LCD ��ʾ��������
#define MASTER "Master bedroom"
#define SECOND "Second bedroom"

//LCD������ʾ
#define WARNING "Warning!!!"
#define NOTHING "          "

unsigned char bytes[4];//�����ֽڻ�������

//���Ի�������
char master_temp[30];
float m_m=0.0;
char master_humi[30];
float m_h=0.0;
char master_co2[30];
float m_c=0.0;
char master_pm[30];
float m_p=0.0;
char master_light[30];
float m_l=0.0;

//���Ի�������
char second_temp[30];
float s_m=0.0;
char second_humi[30];
float s_h=0.0;
char second_co2[30];
float s_c=0.0;
char second_pm[30];
float s_p=0.0;
char second_light[30];
float s_l=0.0;

/*
*********************************************************************************************************
*	�� �� ��: bytetofloat
*	����˵��: 
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
float bytetofloat(uint8_t bytes[])
{
    // ���� bytes ���鳤��Ϊ 4������IEEE 754��׼���������ȸ�����
    uint32_t data = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    float result;
    // �� uint32_t ת��Ϊ float
    *((uint32_t *)&result) = data;
    return result;
}

/*
*********************************************************************************************************
*	�� �� ��: Uarts_Send
*	����˵��: 
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Uarts_Send()
{
	if(ReceiveState)
	{	
		USART4_Send_Data(USART_RX_BUF,RxCounter);
		USART2_Send_Data(USART_RX_BUF,RxCounter);
		Uart0_STA_Clr();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Uarts_Receive
*	����˵��: 
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Uarts_Receive()
{
	uint8_t n;
	
	if(ReceiveState2)
	{
		USART1_Send_Data(USART2_RX_BUF,RxCounter2);
		for(n=1;n<=USART2_RX_BUF[8]/8;n++)
		{
			switch(n)
			{
				case 1:
				{
					bytes[0]=USART2_RX_BUF[8*n+5];
					bytes[1]=USART2_RX_BUF[8*n+6];
					bytes[2]=USART2_RX_BUF[8*n+7];
					bytes[3]=USART2_RX_BUF[8*n+8];
					s_m=bytetofloat(bytes);
				}break;
				case 2:
				{
					bytes[0]=USART2_RX_BUF[8*n+5];
					bytes[1]=USART2_RX_BUF[8*n+6];
					bytes[2]=USART2_RX_BUF[8*n+7];
					bytes[3]=USART2_RX_BUF[8*n+8];
					s_h=bytetofloat(bytes);
				}break;
				case 3:
				{
					bytes[0]=USART2_RX_BUF[8*n+5];
					bytes[1]=USART2_RX_BUF[8*n+6];
					bytes[2]=USART2_RX_BUF[8*n+7];
					bytes[3]=USART2_RX_BUF[8*n+8];
					s_c=bytetofloat(bytes);
				}break;
				case 4:
				{
					bytes[0]=USART2_RX_BUF[8*n+5];
					bytes[1]=USART2_RX_BUF[8*n+6];
					bytes[2]=USART2_RX_BUF[8*n+7];
					bytes[3]=USART2_RX_BUF[8*n+8];
					s_p=bytetofloat(bytes);
				}break;
				case 5:
				{
					if(USART2_RX_BUF[8*n+5]==0x01&&USART2_RX_BUF[8*n+6]==0x01)
					{
						LCD_ShowString(160,270,200,24,24,(uint8_t*)WARNING);
					}
					else
					{
						LCD_ShowString(160,270,200,24,24,(uint8_t*)NOTHING);
					}
				}break;
				case 6:
				{
					bytes[0]=USART2_RX_BUF[8*n+5];
					bytes[1]=USART2_RX_BUF[8*n+6];
					bytes[2]=USART2_RX_BUF[8*n+7];
					bytes[3]=USART2_RX_BUF[8*n+8];
					s_l=bytetofloat(bytes)/100;
				}break;
			}			
		}
		
		Uart2_STA_Clr();
	}
	
	if(ReceiveState4)
	{		
		USART1_Send_Data(USART4_RX_BUF,RxCounter4);
		for(n=1;n<=USART4_RX_BUF[8]/8;n++)
		{
			switch(n)
			{
				case 1:
				{
					bytes[0]=USART4_RX_BUF[8*n+5];
					bytes[1]=USART4_RX_BUF[8*n+6];
					bytes[2]=USART4_RX_BUF[8*n+7];
					bytes[3]=USART4_RX_BUF[8*n+8];
					m_m=bytetofloat(bytes);
				}break;
				case 2:
				{
					bytes[0]=USART4_RX_BUF[8*n+5];
					bytes[1]=USART4_RX_BUF[8*n+6];
					bytes[2]=USART4_RX_BUF[8*n+7];
					bytes[3]=USART4_RX_BUF[8*n+8];
					m_h=bytetofloat(bytes);
				}break;
				case 3:
				{
					bytes[0]=USART4_RX_BUF[8*n+5];
					bytes[1]=USART4_RX_BUF[8*n+6];
					bytes[2]=USART4_RX_BUF[8*n+7];
					bytes[3]=USART4_RX_BUF[8*n+8];
					m_c=bytetofloat(bytes);
				}break;
				case 4:
				{
					bytes[0]=USART4_RX_BUF[8*n+5];
					bytes[1]=USART4_RX_BUF[8*n+6];
					bytes[2]=USART4_RX_BUF[8*n+7];
					bytes[3]=USART4_RX_BUF[8*n+8];
					m_p=bytetofloat(bytes);
				}break;
				case 5:
				{
					if(USART4_RX_BUF[8*n+5]==0x01&&USART4_RX_BUF[8*n+6]==0x01)
					{
						LCD_ShowString(160,150,200,24,24,(uint8_t*)WARNING);
					}
					else
					{
						LCD_ShowString(160,150,200,24,24,(uint8_t*)NOTHING);
					}
				}break;
				case 6:
				{
					bytes[0]=USART4_RX_BUF[8*n+5];
					bytes[1]=USART4_RX_BUF[8*n+6];
					bytes[2]=USART4_RX_BUF[8*n+7];
					bytes[3]=USART4_RX_BUF[8*n+8];
					m_l=bytetofloat(bytes)/100;
				}break;
			}			
		}

		Uart4_STA_Clr();
	}
}

void bsp_InitLcd(void)
{
	TFTLCD_Init(); /* TFTLCD ��ʼ��*/	
	POINT_COLOR=BLUE;//������ɫ
	
	//��ʼ����ӡ
	//����
	LCD_ShowString(10,30,200,24,24,(uint8_t*)DEV_NAME);
	LCD_ShowString(10,60,200,24,24,(uint8_t*)MASTER);
	LCD_ShowString(10,90,200,24,24,(uint8_t*)TEMPERATURE);
	LCD_ShowString(10,120,200,24,24,(uint8_t*)HUMIDITY);
	LCD_ShowString(10,150,200,24,24,(uint8_t*)CO2);
	LCD_ShowString(160,90,200,24,24,(uint8_t*)PM2_5);
	LCD_ShowString(160,120,200,24,24,(uint8_t*)LIGHT);	
	//����
	LCD_ShowString(10,180,200,24,24,(uint8_t*)SECOND);
	LCD_ShowString(10,210,200,24,24,(uint8_t*)TEMPERATURE);
	LCD_ShowString(10,240,200,24,24,(uint8_t*)HUMIDITY);
	LCD_ShowString(10,270,200,24,24,(uint8_t*)CO2);
	LCD_ShowString(160,210,200,24,24,(uint8_t*)PM2_5);
	LCD_ShowString(160,240,200,24,24,(uint8_t*)LIGHT);
	
	bsp_InitRc522(); /* ��ʼ�� RC522 �� SPI �˿� */
	PcdReset(); /* ��λ RC522 */
	M500PcdConfigISOType('A'); /* ���ù�����ʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f10x.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ��
	*/
	bsp_Init();		/* Ӳ����ʼ�� */
	bsp_InitLcd();	
	
	bsp_StartAutoTimer(0,1000);

	while(1)
	{		
		Uarts_Send();
		
		if(bsp_CheckTimer(0))//�ж϶�ʱ����ʱʱ��
		{
			bsp_LedToggle(1);//ÿ500ms����һ�Σ���תled0״̬
			
			Uarts_Receive();			
			
			if(m_m>=0&&s_m>=0)
			{
				//��������
				sprintf(master_temp,"%.1fRT",m_m);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,90,100,24,24,(uint8_t*)master_temp);//�¶�
				sprintf(master_humi,"%.1fRH",m_h);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,120,100,24,24,(uint8_t*)master_humi);//ʪ��
				sprintf(master_co2,"%.0fPPM",m_c);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,150,100,24,24,(uint8_t*)master_co2);//co2	
				sprintf(master_pm,"%.0fPPM",m_p);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(235,90,100,24,24,(uint8_t*)master_pm);//pm		
				sprintf(master_light,"%.1fLUX",m_l);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(235,120,100,24,24,(uint8_t*)master_light);//����		
				//��������
				sprintf(second_temp,"%.1fRT",s_m);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,210,100,24,24,(uint8_t*)second_temp);
				sprintf(second_humi,"%.1fRH",s_h);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,240,100,24,24,(uint8_t*)second_humi);
				sprintf(second_co2,"%.0fPPM",s_c);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(75,270,100,24,24,(uint8_t*)second_co2);
				sprintf(second_pm,"%.0fPPM",s_p);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(235,210,100,24,24,(uint8_t*)second_pm);
				sprintf(second_light,"%.1fLUX",s_l);//�����ڶ�ȡ��������ת��Ϊ�ַ�����ʽ��ӡ
				LCD_ShowString(235,240,100,24,24,(uint8_t*)second_light);
			}			
		}
	}
}

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
