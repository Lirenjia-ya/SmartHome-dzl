/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 按键例子。
*	修改记录 :
*		版本号  日期       作者     说明
*		V1.0    2024-03-06 waroyal  首发
*
*	Copyright (C), 2014-2019, 天津德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */

/* 定义 LCD 显示相关公共信息 */
#define DEV_NAME 				"SmartHome"
#define TEMPERATURE 		"temp:"
#define HUMIDITY			 	"humi:"
#define CO2							"co2 :"
#define PM2_5						"pm2.5:"
#define LIGHT						"light:"

//定义 LCD 显示房间数据
#define MASTER "Master bedroom"
#define SECOND "Second bedroom"

//LCD报警提示
#define WARNING "Warning!!!"
#define NOTHING "          "

unsigned char bytes[4];//接收字节缓存数组

//主卧缓存数据
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

//次卧缓存数据
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
*	函 数 名: bytetofloat
*	功能说明: 
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
float bytetofloat(uint8_t bytes[])
{
    // 假设 bytes 数组长度为 4，按照IEEE 754标准，即单精度浮点数
    uint32_t data = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    float result;
    // 将 uint32_t 转换为 float
    *((uint32_t *)&result) = data;
    return result;
}

/*
*********************************************************************************************************
*	函 数 名: Uarts_Send
*	功能说明: 
*	形    参：
*	返 回 值: 
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
*	函 数 名: Uarts_Receive
*	功能说明: 
*	形    参：
*	返 回 值: 
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
	TFTLCD_Init(); /* TFTLCD 初始化*/	
	POINT_COLOR=BLUE;//字体颜色
	
	//初始化打印
	//主卧
	LCD_ShowString(10,30,200,24,24,(uint8_t*)DEV_NAME);
	LCD_ShowString(10,60,200,24,24,(uint8_t*)MASTER);
	LCD_ShowString(10,90,200,24,24,(uint8_t*)TEMPERATURE);
	LCD_ShowString(10,120,200,24,24,(uint8_t*)HUMIDITY);
	LCD_ShowString(10,150,200,24,24,(uint8_t*)CO2);
	LCD_ShowString(160,90,200,24,24,(uint8_t*)PM2_5);
	LCD_ShowString(160,120,200,24,24,(uint8_t*)LIGHT);	
	//次卧
	LCD_ShowString(10,180,200,24,24,(uint8_t*)SECOND);
	LCD_ShowString(10,210,200,24,24,(uint8_t*)TEMPERATURE);
	LCD_ShowString(10,240,200,24,24,(uint8_t*)HUMIDITY);
	LCD_ShowString(10,270,200,24,24,(uint8_t*)CO2);
	LCD_ShowString(160,210,200,24,24,(uint8_t*)PM2_5);
	LCD_ShowString(160,240,200,24,24,(uint8_t*)LIGHT);
	
	bsp_InitRc522(); /* 初始化 RC522 的 SPI 端口 */
	PcdReset(); /* 复位 RC522 */
	M500PcdConfigISOType('A'); /* 设置工作方式 */
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f10x.c 文件，主要功能是
	配置CPU系统的时钟
	*/
	bsp_Init();		/* 硬件初始化 */
	bsp_InitLcd();	
	
	bsp_StartAutoTimer(0,1000);

	while(1)
	{		
		Uarts_Send();
		
		if(bsp_CheckTimer(0))//判断定时器超时时间
		{
			bsp_LedToggle(1);//每500ms进来一次，翻转led0状态
			
			Uarts_Receive();			
			
			if(m_m>=0&&s_m>=0)
			{
				//主卧数据
				sprintf(master_temp,"%.1fRT",m_m);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,90,100,24,24,(uint8_t*)master_temp);//温度
				sprintf(master_humi,"%.1fRH",m_h);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,120,100,24,24,(uint8_t*)master_humi);//湿度
				sprintf(master_co2,"%.0fPPM",m_c);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,150,100,24,24,(uint8_t*)master_co2);//co2	
				sprintf(master_pm,"%.0fPPM",m_p);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(235,90,100,24,24,(uint8_t*)master_pm);//pm		
				sprintf(master_light,"%.1fLUX",m_l);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(235,120,100,24,24,(uint8_t*)master_light);//光照		
				//次卧数据
				sprintf(second_temp,"%.1fRT",s_m);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,210,100,24,24,(uint8_t*)second_temp);
				sprintf(second_humi,"%.1fRH",s_h);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,240,100,24,24,(uint8_t*)second_humi);
				sprintf(second_co2,"%.0fPPM",s_c);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(75,270,100,24,24,(uint8_t*)second_co2);
				sprintf(second_pm,"%.0fPPM",s_p);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(235,210,100,24,24,(uint8_t*)second_pm);
				sprintf(second_light,"%.1fLUX",s_l);//将串口读取到的数据转换为字符串形式打印
				LCD_ShowString(235,240,100,24,24,(uint8_t*)second_light);
			}			
		}
	}
}

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
