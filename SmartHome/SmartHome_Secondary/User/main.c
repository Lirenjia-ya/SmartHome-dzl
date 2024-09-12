/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 跑马灯例子。使用了systick中断实现精确定时，控制LED指示灯闪烁频率。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-08-02 waroyal  首发
*
*	Copyright (C), 2014-2019, 天津德致伦电子
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */
#include "uart1_cmd.h"

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
	uint16_t co2_value=0;
	uint16_t pm25_value=0;
	float light_value=0;
	
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f10x.c 文件，主要功能是
	配置CPU系统的时钟
	*/
	bsp_Init();		/* 硬件初始化 */
	bsp_InitI2C();
	bsp_InitSht3x();
	bsp_InitIOSensor();
	bsp_InitOpt3001();
	bsp_InitRelay(); 	/* 初始化继电器 */
	bsp_InitFan();		/* 初始化风扇 */
	
	while(bsp_InitOpt3001())//检测不到 OPT3001 ( OPT3001 初始化，即 IIC 初始化和器件 ID 读取 )
	{
		delay_ms(1000);
	}

	bsp_StartAutoTimer(0,1000);
	
	while(1)
	{
		bsp_Idle(); /* CPU 空闲时执行的函数，在 bsp.c */
		
		/* 处理串口 1 逻辑事件 */
		actuator_Data_Receive();
		
		if (bsp_CheckTimer(0)) /* 定时到 */
		{
			bsp_LedToggle(1);		
			
			co2_value=CO2_READ();
			pm25_value=PM25_READ();
			light_value=(float)GetLuxValue();

			if(SHT3X_GetValue())
			{
				bsp_InitI2C();
				bsp_InitSht3x();
			}
			else
			{
				if(co2_value!=65535&&pm25_value!=65535)
				{	
					//字节协议发送数据
					sensor_Data_Upload(Tem_Value,RH_Value,co2_value,pm25_value,light_value);
				}				
			}
			
			if(PIR_OUT_flag==1)
			{
				PIR_OUT_flag=0;//清零	
				bsp_BeepOff();
			}
			
			if(VIBRATE_OUT_flag==1)
			{
				VIBRATE_OUT_flag=0;//清零
				bsp_BeepOff();
			}
		}		
	}
}

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
