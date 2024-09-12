/*
*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ��������ӡ�ʹ����systick�ж�ʵ�־�ȷ��ʱ������LEDָʾ����˸Ƶ�ʡ�
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-08-02 waroyal  �׷�
*
*	Copyright (C), 2014-2019, �������׵���
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */
#include "uart1_cmd.h"

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
	uint16_t co2_value=0;
	uint16_t pm25_value=0;
	float light_value=0;
	
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f10x.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ��
	*/
	bsp_Init();		/* Ӳ����ʼ�� */
	bsp_InitI2C();
	bsp_InitSht3x();
	bsp_InitIOSensor();
	bsp_InitOpt3001();
	bsp_InitRelay(); 	/* ��ʼ���̵��� */
	bsp_InitFan();		/* ��ʼ������ */
	
	while(bsp_InitOpt3001())//��ⲻ�� OPT3001 ( OPT3001 ��ʼ������ IIC ��ʼ�������� ID ��ȡ )
	{
		delay_ms(1000);
	}

	bsp_StartAutoTimer(0,1000);
	
	while(1)
	{
		bsp_Idle(); /* CPU ����ʱִ�еĺ������� bsp.c */
		
		/* ������ 1 �߼��¼� */
		actuator_Data_Receive();
		
		if (bsp_CheckTimer(0)) /* ��ʱ�� */
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
					//�ֽ�Э�鷢������
					sensor_Data_Upload(Tem_Value,RH_Value,co2_value,pm25_value,light_value);
				}				
			}
			
			if(PIR_OUT_flag==1)
			{
				PIR_OUT_flag=0;//����	
				bsp_BeepOff();
			}
			
			if(VIBRATE_OUT_flag==1)
			{
				VIBRATE_OUT_flag=0;//����
				bsp_BeepOff();
			}
		}		
	}
}

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
