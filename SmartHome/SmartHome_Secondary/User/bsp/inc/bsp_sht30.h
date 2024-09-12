/*
*********************************************************************************************************
*
*	ģ������ : ��ʪ�ȴ�����ģ��
*	�ļ����� : bsp_sht30.h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    x2024-05-31
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_SHT30_H_
#define _BSP_SHT30_H_

///////////////////////////////////////
/* ͷ�ļ������� */

#include "bsp.h"


///////////////////////////////////////
/* �궨������ */
#define i2cAddWrite_8bit 			0x88
#define i2cAddRead_8bit 			0x89
/*CRC*/
#define POLYNOMIAL 					0x131 
// Sensor Commands

typedef enum{
	CMD_READ_SERIALNBR = 0x3780, 
	CMD_READ_STATUS = 0xF32D, 
	CMD_CLEAR_STATUS = 0x3041,
	CMD_HEATER_ENABLE = 0x306D, 
	CMD_HEATER_DISABLE = 0x3066,
	CMD_SOFT_RESET = 0x30A2, 
	CMD_MEAS_CLOCKSTR_H = 0x2C06, 
	CMD_MEAS_CLOCKSTR_M = 0x2C0D,
	CMD_MEAS_CLOCKSTR_L = 0x2C10, 
	CMD_MEAS_POLLING_H = 0x2400,
	CMD_MEAS_POLLING_M = 0x240B,
	CMD_MEAS_POLLING_L = 0x2416, 
	CMD_MEAS_PERI_05_H = 0x2032, 
	CMD_MEAS_PERI_05_M = 0x2024, 
	CMD_MEAS_PERI_05_L = 0x202F, 
	CMD_MEAS_PERI_1_H = 0x2130,
	CMD_MEAS_PERI_1_M = 0x2126, 
	CMD_MEAS_PERI_1_L = 0x212D, 
	CMD_MEAS_PERI_2_H = 0x2236, 
	CMD_MEAS_PERI_2_M = 0x2220,
	CMD_MEAS_PERI_2_L = 0x222B,
	CMD_MEAS_PERI_4_H = 0x2334, 
	CMD_MEAS_PERI_4_M = 0x2322, 
	CMD_MEAS_PERI_4_L = 0x2329,
	CMD_MEAS_PERI_10_H = 0x2737, 
	CMD_MEAS_PERI_10_M = 0x2721,
	CMD_MEAS_PERI_10_L = 0x272A, 
	CMD_FETCH_DATA = 0xE000, 
	CMD_R_AL_LIM_LS = 0xE102,
	CMD_R_AL_LIM_LC = 0xE109, 
	CMD_R_AL_LIM_HS = 0xE11F,
	CMD_R_AL_LIM_HC = 0xE114,
	CMD_W_AL_LIM_LS = 0x6100, 
	CMD_W_AL_LIM_LC = 0x610B, 
	CMD_W_AL_LIM_HS = 0x611D, 
	CMD_W_AL_LIM_HC = 0x6116,
	CMD_NO_SLEEP = 0x303E,
}etCommands;


extern float Tem_Value;
extern float RH_Value;


//��������
void bsp_InitSht3x(void); //��ʼ��
uint8_t SHT3X_GetValue(void); //��ȡ��������ʪ��ֵ
void SHX3X_ReadResults(uint16_t cmd, uint8_t *p);//�õ�ԭʼ����ʪֵ 6 ���ֽ�




///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */

///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

