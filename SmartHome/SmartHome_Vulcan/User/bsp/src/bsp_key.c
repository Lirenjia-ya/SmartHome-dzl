/*
*********************************************************************************************************
*
*	ģ������ : key����ģ��
*	�ļ����� : bsp_key.c
*	��    �� : Vx.x
*	˵    �� : ɨ�������������������˲����ƣ����а���FIFO
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2024-03-08 waroyal  ��ʽ����
*
*	Copyright (C), 2014-2024, �����׵���
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* ͷ�ļ������� */
#include "bsp.h"

///////////////////////////////////////
/* ���������� */
static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;	//����FIFO�������ṹ��

///////////////////////////////////////
/* �ⲿ���������� */

///////////////////////////////////////
/* ���������� */
static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

///////////////////////////////////////
/* ����ʵ���� */
/*���Ӱ���
*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����1��ʾ���£�0��ʾδ����
*********************************************************************************************************
*/
static uint8_t IsKeyDown1(void)	//WKUP
{
	if((WKUP==WKUP_GPIO_LEVEL)&&(KEY0!=KEY0_GPIO_LEVEL)&&(KEY1!=KEY1_GPIO_LEVEL)&&(KEY2!=KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown2(void)	//KEY0
{
	if((WKUP!=WKUP_GPIO_LEVEL)&&(KEY0==KEY0_GPIO_LEVEL)&&(KEY1!=KEY1_GPIO_LEVEL)&&(KEY2!=KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown3(void)	//KEY1
{
	if((WKUP!=WKUP_GPIO_LEVEL)&&(KEY0!=KEY0_GPIO_LEVEL)&&(KEY1==KEY1_GPIO_LEVEL)&&(KEY2!=KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown4(void)	//KEY2
{
	if((WKUP!=WKUP_GPIO_LEVEL)&&(KEY0!=KEY0_GPIO_LEVEL)&&(KEY1!=KEY1_GPIO_LEVEL)&&(KEY2==KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown5(void)	//WKUP+KEY0
{
	if((WKUP==WKUP_GPIO_LEVEL)&&(KEY0==KEY0_GPIO_LEVEL)&&(KEY1!=KEY1_GPIO_LEVEL)&&(KEY2!=KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}
static uint8_t IsKeyDown6(void)	//KEY0+KEY1
{
	if((WKUP!=WKUP_GPIO_LEVEL)&&(KEY0==KEY0_GPIO_LEVEL)&&(KEY1==KEY1_GPIO_LEVEL)&&(KEY2!=KEY2_GPIO_LEVEL))
		return 1;
	else
		return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ���������ú�����bsp_Init()���ã���Ҫ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();	//��ʼ����������
	bsp_InitKeyHard();	//��ʼ������Ӳ��
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��һ����ֵѹ�밴��FIFO��������������ģ��һ������
*	��    ��: ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write]=_KeyCode;
	
	if(++s_tKey.Write>=KEY_FIFO_SIZE)
	{
		s_tKey.Write=0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ����������ж�ȡһ����ֵ
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;
	
	if(s_tKey.Read==s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret=s_tKey.Buf[s_tKey.Read];
		
		if(++s_tKey.Read==KEY_FIFO_SIZE)
		{
			s_tKey.Read=0;
		}
		return ret;
	}
}

///*
//*********************************************************************************************************
//*	�� �� ��: bsp_GetKey2
//*	����˵��: �Ӱ����������ж�ȡһ����ֵ
//*	��    ��: ��
//*	�� �� ֵ: ��������
//*********************************************************************************************************
//*/
//uint8_t bdp_GetKey2(void)
//{
//	uint8_t ret;
//	
//	if(s_tKey.Read2==s_tKey.Write)
//	{
//		return KEY_NONE;
//	}
//	else
//	{
//		ret=s_tKey.Buf[s_tKey.Write];
//		
//		if(++s_tKey.Read2==KEY_FIFO_SIZE)
//		{
//			s_tKey.Read2=0;
//		}
//		return ret;
//	}
//}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ����״̬
*	��    ��: _ucKeyID������ID����0��ʼ
*	�� �� ֵ: 1��ʾ���£�0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    ��: _ucKeyID������ID����0��ʼ
				_LongTime�������¼�ʱ��
				_RepeatSpeed�������ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID,uint16_t _LongTime,uint8_t _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime=_LongTime;
	s_tBtn[_ucKeyID].RepeatSpeed=_RepeatSpeed;
	s_tBtn[_ucKeyID].RepeatCount=0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read=s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyhHard
*	����˵��: ���ð�����ص�GPIO���ú����� bsp_Init() ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	//1.����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�jtag
	
	//3.�ṹ�帳ֵ
	GPIO_InitStructure.GPIO_Pin=WKUP_GPIO_PIN;//WKUP�˿�
	GPIO_InitStructure.GPIO_Mode=WKUP_GPIO_MODE;//WKUP�˿�ģʽ
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure);//��ʼ��WKUP
	
	GPIO_InitStructure.GPIO_Pin=KEY0_GPIO_PIN;//KEY0�˿�
	GPIO_InitStructure.GPIO_Mode=KEY0_GPIO_MODE;//KEY0�˿�ģʽ
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure);//��ʼ��KEY0
	
	GPIO_InitStructure.GPIO_Pin=KEY1_GPIO_PIN;//KEY1�˿�
	GPIO_InitStructure.GPIO_Mode=KEY1_GPIO_MODE;//KEY1�˿�ģʽ
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);//��ʼ��KEY1
	
	GPIO_InitStructure.GPIO_Pin=KEY2_GPIO_PIN;//KEY2�˿�
	GPIO_InitStructure.GPIO_Mode=KEY2_GPIO_MODE;//KEY2�˿�ģʽ
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);//��ʼ��KEY2	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;
	
	/*�԰���FIFO��дָ������*/
	s_tKey.Read=0;
	s_tKey.Write=0;
//	s_tKey.Read2=0;
	
	/*��ÿ�������ṹ���Ա������һ��ȱʡֵ*/
	for(i=0;i<KEY_COUNT;i++)
	{
		s_tBtn[i].LongTime=KEY_LONG_TIME;	//����ʱ��0��ʾ����ⳤ�����¼�
		s_tBtn[i].Count=KEY_FILTER_TIME/2;	//����������Ϊ�˲���ʱ���һ��
		s_tBtn[i].State=0;					//����ȱʡ״̬��0Ϊδ����
		s_tBtn[i].RepeatSpeed=0;			//�����������ٶȣ�0��ʾ��֧������
		s_tBtn[i].RepeatCount=0;			//����������
	}
	
	/*�����Ҫ�޸�ĳ�������Ĳ�����ֱ���ض���*/
	//s_tBtn[KID_K1].LongTime=100;
	//s_tBtn[KID_K1].RepeatSpeed=5;	ÿ��50ms�Զ����ͼ�ֵ
	
	/*�жϰ������µĺ���*/
	s_tBtn[0].IsKeyDownFunc=IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc=IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc=IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc=IsKeyDown4;
	/*��ϼ�*/
	s_tBtn[4].IsKeyDownFunc=IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc=IsKeyDown6;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ���
*	��    ��: �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	
	pBtn=&s_tBtn[i];
	if(pBtn->IsKeyDownFunc())
	{
		//ִ�а������µĴ���
		if(pBtn->Count<KEY_FILTER_TIME)
		{
			pBtn->Count=KEY_FILTER_TIME;
		}
		else if(pBtn->Count<2*KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if(pBtn->State==0)
			{
				pBtn->State=1;
				//���Ͱ�ť���µ���Ϣ
				bsp_PutKey((uint8_t)(3*i+1));
			}
			
			if(pBtn->LongTime>0)
			{
				if(pBtn->LongCount<pBtn->LongTime)
				{
					//���Ͱ�ť�������µ���Ϣ
					if(++pBtn->LongCount==pBtn->LongTime)
					{
						//��ֵ���밴��FIFO
						bsp_PutKey((uint8_t)(3*i+3));
					}
				}
				else
				{
					if(pBtn->RepeatSpeed>0)
					{
						if(++pBtn->RepeatCount>=pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount=0;
							//��������ÿ��10ms����һ������
							bsp_PutKey((uint8_t)(3*i+1));
						}
					}
				}
			}
		}
	}
	else
	{
		//ִ��δ���°����򰴼����ֵĴ���
		if(pBtn->Count>KEY_FILTER_TIME)
		{
			pBtn->Count=KEY_FILTER_TIME;
		}
		else if(pBtn->Count!=0)
		{
			pBtn->Count--;
		}
		else
		{
			if(pBtn->State==1)
			{
				pBtn->State=0;
				//���Ͱ����������Ϣ
				bsp_PutKey((uint8_t)(3*i+2));
			}
		}
		
		pBtn->LongCount=0;
		pBtn->RepeatCount=0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;
	
	for(i=0;i<KEY_COUNT;i++)
	{
		bsp_DetectKey(i);
	}
}

/*�򵥰���-------------------------------------------------------------------------------------------------------------------------------------------------------------------
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ��������
*	��    ��: ��
*	�� �� ֵ: WKUP_NUM��KEY0_NUM��KEY1_NUM��KEY2_NUM
*********************************************************************************************************
*/
/*
uint8_t bsp_KeyScan(void)
{
	if(WKUP==WKUP_GPIO_LEVEL)
	{
		delay_ms(10);
		if(WKUP==WKUP_GPIO_LEVEL)
		{
			while(WKUP==WKUP_GPIO_LEVEL);
			return WKUP_NUM;
		}
		else
		{
			return NOKEY_NUM;
		}
	}
	
	else if(KEY0==KEY0_GPIO_LEVEL)
	{
		delay_ms(10);
		if(KEY0==KEY0_GPIO_LEVEL)
		{
			while(KEY0==KEY0_GPIO_LEVEL);
			return KEY0_NUM;
		}
		else
		{
			return NOKEY_NUM;
		}
	}
	
	else if(KEY1==KEY1_GPIO_LEVEL)
	{
		delay_ms(10);
		if(KEY1==KEY1_GPIO_LEVEL)
		{
			while(KEY1==KEY1_GPIO_LEVEL);
			return KEY1_NUM;
		}
		else
		{
			return NOKEY_NUM;
		}
	}
	
	else if(KEY2==KEY2_GPIO_LEVEL)
	{
		delay_ms(10);
		if(KEY2==KEY2_GPIO_LEVEL)
		{
			while(KEY2==KEY2_GPIO_LEVEL);
			return KEY2_NUM;
		}
		else
		{
			return NOKEY_NUM;
		}
	}
	
	return NOKEY_NUM;
}
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

///////////////////////////////////////

/***************************** �����׵��� DeZLinc (END OF FILE) *********************************/
