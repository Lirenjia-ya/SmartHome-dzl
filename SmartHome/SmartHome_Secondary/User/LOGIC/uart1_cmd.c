/*
*********************************************************************************************************
*
*	ģ������ : actuator_Data_Receive����ģ��
*	�ļ����� : actuator_Data_Receive.c
*	��    �� : Vx.x
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-06-18  waroyal  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

//ִ����ģ����λ��->��λ���������� ������Ϊ0x01
/*
	USART_RX_BUF[]={							//����1�Ľ��ջ�������
		//֡ͷ
		0x41,0x73,0x75,0x6b,0x61,		//Asuka 41 73 75 6B 61
		//�������ĳ���
		0x05,												//len-FRAME_CONST_BYTE_LEN
		//������
		0x01,												//��ַ�� ����-0x01 ����-0x02 ����-0x03
		0x01,												//�̵�������-0x01 or ��������-0x02
		0x01,												//�̵�����(0x01~0x04) or ���Ⱥ�(0x01~0x02) 
		0x01,												//��ת����
		0x01,												//״̬ ��-0x00 ��-0x01
		//У��λ
		0x06												//�������ֽ��ۼӺ�
	};
*/

//ִ����ģ����λ��->��λ�����ͷ������� ������Ϊ0x02
/*
	XXXXX_RSP_BYTE[]={						//����1�Ľ��ջ�������
		//֡ͷ
		0x41,0x73,0x75,0x6b,0x61,		//Asuka 41 73 75 6B 61
		//�������ĳ���
		0x05,												//len-FRAME_CONST_BYTE_LEN
		//������
		0x01,												//��ַ�� ����-0x01 ����-0x02 ����-0x03
		0x01,												//�̵�������-0x01 or ��������-0x02
		0x01,												//�̵�����(0x01~0x04) or ���Ⱥ�(0x01~0x02) 
		0x01,												//��ת����
		0x01,												//״̬ ��-0x00 ��-0x01
		//У��λ
		0x07												//�������ֽ��ۼӺ�
	};
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "uart1_cmd.h"

///////////////////////////////////////
/* ���������� */
//�趨��������Ϣ�ϱ��Ļ���
uint8_t SENSOR_DATA_BYTE[248] ={FRAME_HEADER_BYTE_1,FRAME_HEADER_BYTE_2,FRAME_HEADER_BYTE_3,FRAME_HEADER_BYTE_4,FRAME_HEADER_BYTE_5,//֡ͷ��5 ���ֽڣ�
														0x35, //���������ȣ�1 ���ֽڣ�
														0x01, //����Ϊ���ݣ�1 ���ֽڣ�
														0x01, //Ϊ���������ݣ�1 ���ֽڣ�
														0x10, //���������ݳ��ȣ�1 ���ֽڣ�
														//---- //��������������8*N ���ֽڣ���N ���Ϊ 30��
														//- //У�飨1 ���ֽڣ�
														//- //��ַ��
														};
														
_sensorData Sensor_Data_ToFloat;//����������ת��Ϊ������
_sensorData Sensor_Data_To4Byte;//����������ת��Ϊ 4 �ֽ� �ڴ�

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
/* ����ʵ���� */
/*
*********************************************************************************************************
*	�� �� ��: CheckSum
*	����˵��: ����У���
*	��    �Σ�uint8_t USART_RX_BUF[]:����������
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t CheckSum(uint8_t USART_RX_BUF[],uint8_t len)
{
	uint8_t sum=0;
	uint8_t i;
	for(i=0;i<len;i++)
	{
		sum+=USART_RX_BUF[i];
	}
	
	return sum;
}

/*
*********************************************************************************************************
*	�� �� ��: actuator_Data_Receive
*	����˵��: ���մ������� �������ݿ���ִ���� ������״̬ͬʱͨ�����ڷ��͸���λ��
*	��    �Σ�void
*	�� �� ֵ: void
*********************************************************************************************************
*/
void actuator_Data_Receive(void)
{
	u16 len = 0;//�����ݴ�
	
	if ( ReceiveState )//����1���յ�����
	{
		len = RxCounter;
		
		//����֡ͷ
		if(USART_RX_BUF[0]==FRAME_HEADER_BYTE_1 && USART_RX_BUF[1]==FRAME_HEADER_BYTE_2 && USART_RX_BUF[2]==FRAME_HEADER_BYTE_3 && USART_RX_BUF[3]==FRAME_HEADER_BYTE_4 && USART_RX_BUF[4]==FRAME_HEADER_BYTE_5 )
		{
			//�ж�У����Ƿ���ȷ
			if(USART_RX_BUF[len-1]==CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))
			{
				//�ж����ݳ����Ƿ���ȷ
				if(USART_RX_BUF[5]==len-FRAME_CONST_BYTE_LEN)
				{
					//�жϵ�ַ�� ����-0x02
					if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET]==ADDRESS_ROOM)
					{
						//�жϾ������� ��ת
						if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3]==FUN_CODE_CMD_RELAY_TOGGLE)
						{
							//�ж��Ǽ̵�������Ƿ�������
							switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+1])
							{
								case FUN_CODE_DEVICE_RELAY://�̵�������
								{
									//��ת�̵���
									bsp_RelayToggle(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
								}break;
								case FUN_CODE_DEVICE_FAN://��������
								{
									if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+4]==0x00)//����״̬Ϊ��
									{
										bsp_FanOn(1);
										bsp_FanOn(2);
									}else//����״̬Ϊ��
									{
										bsp_FanOff(1);
										bsp_FanOff(2);
									}			
								}break;
							}
						}
					}
				}
			}		
		}
		Uart_STA_Clr();//����1״̬���
	}
}


/*
*********************************************************************************************************
* �� �� ����sensor_TempHumi_Upload
* ����˵������ʪ�ȴ������ֽ�Э���ϱ�����
* ��    �Σ�float temperature:�¶ȸ���������
* 					float humidity:ʪ�ȸ���������
* �� �� ֵ����
*********************************************************************************************************
*/ 
void sensor_Data_Upload(float temperature,float humidity,float co2,float pm25,float light)
{
	uint8_t i = 0;//ѭ������
	//����һ���¶ȴ�����
	uint8_t sensor_Temp_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_TEMP_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};
		
		
	//����һ��ʪ�ȴ�����
	uint8_t sensor_Humi_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_HUMI_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};
	

	//����һ��co2������
	uint8_t sensor_co2_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_CO2_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};
	
	//����һ��pm2.5������
	uint8_t sensor_pm25_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_PM25_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};
	
	//����һ��shock pir������
	uint8_t sensor_shock_pir_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_SHOCK_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};
	
	//����һ�¹��մ�����
	uint8_t sensor_light_buf[8] = {
													ENVIRONMENT_SENSOR_TYPE_CODE_MSB,		
													ENVIRONMENT_SENSOR_TYPE_CODE_LSB,	
													SENSOR_LM_CODE,	
													SENSOR_STATUS_RUN,
													0,
													0,
													0,
													0
								};

	//��ֵ��ע��Թ�����ĵĸ�������ֵ
	Sensor_Data_To4Byte.f = temperature;
	//ע�⣬�ڴ���������С��ģʽ�洢�ģ������Ҫת��Ϊ���ģʽ
	for(i=0;i<4;i++)
	{
		sensor_Temp_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = humidity;
	for(i=0;i<4;i++)
	{
		sensor_Humi_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = co2;
	for(i=0;i<4;i++)
	{
		sensor_co2_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	Sensor_Data_To4Byte.f = pm25;
	for(i=0;i<4;i++)
	{
		sensor_pm25_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	sensor_shock_pir_buf[4]=VIBRATE_OUT_flag;
	sensor_shock_pir_buf[5]=PIR_OUT_flag;
	
	Sensor_Data_To4Byte.f = light;
	for(i=0;i<4;i++)
	{
		sensor_light_buf[4 + i] = Sensor_Data_To4Byte.x[3-i];
	}
	
	//ƫ�Ƶ�ַ 0x05
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 51;//( 1 + 1 + 1 + 6 * 8 )���� 43 �ֽ�
	//ƫ�Ƶ�ַ 0x06
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA; //����֡
	//ƫ�Ƶ�ַ 0x07
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR; //����������
	//ƫ�Ƶ�ַ 0x08
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 48; //���� 40 ���ֽ�
	//ƫ�Ƶ�ַ 0x09
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 0 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Temp_buf,8);//�����¶�����
	//ƫ�Ƶ�ַ 0x11
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 1 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Humi_buf,8);//����ʪ������
	//ƫ�Ƶ�ַ 0x19
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_co2_buf,8);//����co2����
	//ƫ�Ƶ�ַ 0x21
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 3 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_pm25_buf,8);//����PM2.5����	
	//ƫ�Ƶ�ַ 0x29
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 4 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_shock_pir_buf,8);//����������&��������
	//ƫ�Ƶ�ַ 0x31
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 5 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_light_buf,8);//������������
	
	//����У���
	SENSOR_DATA_BYTE[( SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN ) )] = CheckSum( &SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET] );// ����У����;
	//��ַ��
	SENSOR_DATA_BYTE[( SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN ) )+1] = ADDRESS_ROOM;
	
	USART1_Send_Data(SENSOR_DATA_BYTE,59);//ͨ������1���ʹ��������ݵ�����
}

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
