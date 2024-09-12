/*
*********************************************************************************************************
*
*	ģ������ : usart1_cmdģ��
*	�ļ����� : usart1_cmd.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    xxx-xx-xx
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef __UART1_CMD_H_
#define __UART1_CMD_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

///////////////////////////////////////
/* �궨������ */
//֡ͷ
#define	FRAME_HEADER_BYTE_1				0x41		//A
#define	FRAME_HEADER_BYTE_2				0x73		//s
#define	FRAME_HEADER_BYTE_3				0x75		//u
#define	FRAME_HEADER_BYTE_4				0x6b		//k
#define	FRAME_HEADER_BYTE_5				0x61		//a

//�������ĳ���
#define	FRAME_CONST_BYTE_LEN			(5+1+1)	//�������������ݳ��� ֡ͷ+���ݳ���+У����

//������
#define	FRAME_DATA_FIELD_OFFSET		6				//�������Ŀ�ͷƫ�Ƶ�ַ Ҳ���ǵ�ַ��

//��ַ��
#define	ADDRESS_ROOM							0x01		//����

//����
#define	FUN_CODE_DEVICE_RELAY			0x01		//�̵�������
#define	FUN_CODE_DEVICE_FAN				0x02		//��������

//�̵�����
#define	FUN_DEVICE_RELAY1					0x01		//�̵���1
#define	FUN_DEVICE_RELAY2					0x02		//�̵���2
#define	FUN_DEVICE_RELAY3					0x03		//�̵���3
#define	FUN_DEVICE_RELAY4					0x04		//�̵���4

//���Ⱥ�
#define	FUN_DEVICE_FAN1						0x01		//����1
#define	FUN_DEVICE_FAN2						0x02		//����2

//��������
#define	FUN_CODE_CMD_RELAY_TOGGLE	0x01		//�̵�����ת����
#define	FUN_CODE_CMD_FAN_TOGGLE		0x01		//���ȷ�ת����

//�̵���״̬
#define	FUN_CODE_RSP_RELAY_OFF		0x01		//�̵�����
#define	FUN_CODE_RSP_RELAY_ON			0x00		//�̵�����

//����״̬
#define	FUN_CODE_RSP_FAN_OFF			0x01		//���ȹ�
#define	FUN_CODE_RSP_FAN_ON				0x00		//���ȿ�


//����֡		
#define FUN_CODE_DATA 										0x01		//������ ���ʹ���������
//���������������ݳ���
#define SENSOR_SINGLE_DATA_LEN 						8

#define DATA_LEN_OFFSET 									5 			//���ݳ���ƫ�Ƶ�ַ
#define FRAME_TYPE_OFFSET 								6 			//֡����ƫ�Ƶ�ַ
#define DATA_TYPE_OFFSET 									7 			//����֡����������ƫ�Ƶ�ַ
#define SENSOR_DATA_LEN_OFFSET 						8 			//���������ݳ��� ƫ�Ƶ�ַ
#define SENSOR_DATA_START_OFFSET 					9 			//��һ�������������׵�ַ ƫ�Ƶ�ַ
#define SENSOR_TEMP_CODE 									0x01 		//�¶ȴ�����
#define SENSOR_HUMI_CODE 									0x02		//ʪ�ȴ�����
#define SENSOR_LM_CODE 										0x04 		//���մ�����
#define SENSOR_CO2_CODE 									0x05 		//CO2 ������
#define SENSOR_PM25_CODE 									0x06 		//PM2.5 ������

#define SENSOR_SHOCK_CODE 								0x07 		//�𶯴����� & ���⴫����

#define ENVIRONMENT_SENSOR_TYPE_CODE_MSB 	0x22 		//����������
#define ENVIRONMENT_SENSOR_TYPE_CODE_LSB 	0x00	 	//
#define SENSOR_STATUS_PRE 								0x00	 	//������Ԥ��
#define SENSOR_STATUS_NG 									0x01 		//��������ֵ�쳣
#define SENSOR_STATUS_RUN 								0x02 		//��������������
#define DATA_TYPE_SENSOR 									0x01 		//����������

//ע�� x[3]Ϊ���,x[0]ΪС��
typedef union
{
	float f;
	uint8_t x[4];
}_sensorData;//���������� float �� 4 �ֽ� �໥�任 ʹ��������

///////////////////////////////////////
/* �ⲿ���������� */
extern uint8_t SENSOR_DATA_BYTE[248];

///////////////////////////////////////
/* ���������� */
void actuator_Data_Receive(void);
uint8_t CheckSum(uint8_t USART_RX_BUF[],uint8_t len);

//�ֽ���ʽ������ʪ����Ϣ
void sensor_Data_Upload(float temperature,float humidity,float co2,float pm25,float light);

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

