/*
*********************************************************************************************************
*
*	ģ������ : ģ��
*	�ļ����� : .h
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-08
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

///////////////////////////////////////
/* �궨������ */
#define RCC_ALL_KEY 	(WKUP_GPIO_CLK|KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK)
//WKUP--------------------------------------------------------------------
#define WKUP_GPIO_PIN		GPIO_Pin_0				//WKUP���ź�
#define WKUP_PIN_ID			0						//WKUP�������
#define WKUP_GPIO_PORT		GPIOA					//WKUP�˿ں�
#define WKUP_GPIO_CLK		RCC_APB2Periph_GPIOA	//WKUPʱ��
//#define WKUP_FUN_OUT		PAout					//WKUP����˿����ú���
#define WKUP_FUN_IN			PAin					//WKUP����˿����ú���
#define WKUP_GPIO_MODE		GPIO_Mode_IPD			//WKUP����ģʽ
#define WKUP_GPIO_LEVEL		1						//WKUP����Ч��ƽ

//KEY0--------------------------------------------------------------------
#define KEY0_GPIO_PIN		GPIO_Pin_4				//KEY0���ź�
#define KEY0_PIN_ID			4						//KEY0�������
#define KEY0_GPIO_PORT		GPIOE					//KEY0�˿ں�
#define KEY0_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY0ʱ��
//#define WKUP_FUN_OUT		PEout					//KEY0����˿����ú���
#define KEY0_FUN_IN			PEin					//KEY0����˿����ú���
#define KEY0_GPIO_MODE		GPIO_Mode_IPU			//KEY0����ģʽ
#define KEY0_GPIO_LEVEL		0						//KEY0����Ч��ƽ

//KEY1--------------------------------------------------------------------
#define KEY1_GPIO_PIN		GPIO_Pin_3				//KEY1���ź�
#define KEY1_PIN_ID			3						//KEY1�������
#define KEY1_GPIO_PORT		GPIOE					//KEY1�˿ں�
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY1ʱ��
//#define WKUP_FUN_OUT		PEout					//KEY1����˿����ú���
#define KEY1_FUN_IN			PEin					//KEY1����˿����ú���
#define KEY1_GPIO_MODE		GPIO_Mode_IPU			//KEY1����ģʽ
#define KEY1_GPIO_LEVEL		0						//KEY1����Ч��ƽ

//KEY2--------------------------------------------------------------------
#define KEY2_GPIO_PIN		GPIO_Pin_2				//KEY2���ź�
#define KEY2_PIN_ID			2						//KEY2�������
#define KEY2_GPIO_PORT		GPIOE					//KEY2�˿ں�
#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOE	//KEY2ʱ��
//#define WKUP_FUN_OUT		PEout					//KEY2����˿����ú���
#define KEY2_FUN_IN			PEin					//KEY2����˿����ú���
#define KEY2_GPIO_MODE		GPIO_Mode_IPU			//KEY2����ģʽ
#define KEY2_GPIO_LEVEL		0						//KEY2����Ч��ƽ

//IO��������	 
#define WKUP    WKUP_FUN_IN(WKUP_PIN_ID) //WKUP
#define KEY0    KEY0_FUN_IN(KEY0_PIN_ID) //KEY0
#define KEY1    KEY1_FUN_IN(KEY1_PIN_ID) //KEY1
#define KEY2    KEY2_FUN_IN(KEY2_PIN_ID) //KEY2


/*�򵥰�������ֵ-----------------------------------------------------------------------------------------------------------------------------------------------
#define NOKEY_NUM	0
#define WKUP_NUM	1	//WKUP
#define KEY0_NUM	2	//KEY0
#define KEY1_NUM	3	//KEY1
#define KEY2_NUM	4	//KEY2
-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define KEY_COUNT 6	//����������4����������+2����ϼ�

/*����Ӧ�ó���Ĺ���������������*/
#define WKUP_DOWN 	KEY_1_DOWN
#define WKUP_UP 	KEY_1_UP
#define WKUP_LONG 	KEY_1_LONG

#define KEY0_DOWN 	KEY_2_DOWN
#define KEY0_UP 	KEY_2_UP
#define KEY0_LONG 	KEY_2_LONG

#define KEY1_DOWN 	KEY_3_DOWN
#define KEY1_UP 	KEY_3_UP
#define KEY1_LONG 	KEY_3_LONG

#define KEY2_DOWN 	KEY_4_DOWN
#define KEY2_UP 	KEY_4_UP
#define KEY2_LONG 	KEY_4_LONG

#define SYS_DOWN_WKUP_KEY0 	KEY_5_DOWN	//��ϼ�WKUP+KEY0
#define SYS_UP_WKUP_KEY0 	KEY_5_UP
#define SYS_LONG_WKUP_KEY0 	KEY_5_LONG

#define SYS_DOWN_KEY0_KEY1 	KEY_6_DOWN	//��ϼ�KEY0+KEY1
#define SYS_UP_KEY0_KEY1	KEY_6_UP
#define SYS_LONG_KEY0_KEY1 	KEY_6_LONG


/*����id����Ҫ����bsp_KeyState()������ڲ���*/
typedef enum
{
	KID_K1=0,
	KID_K2,
	KID_K3,
	KID_K4,
}KEY_ID_E;


/*
�����˲�ʱ��50ms����λ10ms
ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/
#define KEY_FILTER_TIME 5
#define KEY_LONG_TIME 	100	//	��λ10ms������1s����Ϊ�����¼�



/*�����ֵ����*/
typedef enum
{
	KEY_NONE=0,	//0��ʾ�����¼�
	
	KEY_1_DOWN,	//����
	KEY_1_UP,	//����
	KEY_1_LONG,	//����
	
	KEY_2_DOWN,
	KEY_2_UP,
	KEY_2_LONG,
	
	KEY_3_DOWN,
	KEY_3_UP,
	KEY_3_LONG,
	
	KEY_4_DOWN,
	KEY_4_UP,
	KEY_4_LONG,
	
	//��ϼ�
	KEY_5_DOWN,
	KEY_5_UP,
	KEY_5_LONG,
	
	KEY_6_DOWN,
	KEY_6_UP,
	KEY_6_LONG,
}KEY_ENUM;


/*�����ṹ���Ա*/
typedef struct
{
	uint8_t (*IsKeyDownFunc)(void);	//�������µ��жϺ�����1��ʾ����
	
	uint8_t Count;					//�˲�����������
	uint16_t LongCount;				//����������
	uint16_t LongTime;				//�������³���ʱ�䣬��ʼ��0ʱ��ʾ����ⳤ��
	uint8_t State;					//������ǰ״̬������or����
	uint8_t RepeatSpeed;			//������������
	uint8_t RepeatCount;			//��������������
}KEY_T;

/*����FIFO�õ��ı���*/
#define KEY_FIFO_SIZE 10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];	//��ֵ������
	uint8_t Read;				//��������ָ��1
	uint8_t Write;				//������дָ��
	uint8_t Read2;				//��������ָ��2
}KEY_FIFO_T;



///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
void bsp_InitKey(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID,uint16_t _LongTime,uint8_t _RepeatSpeed);
void bsp_ClearKey(void);
void bsp_KeyScan(void);
///////////////////////////////////////
#endif

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/

