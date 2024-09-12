/*
*********************************************************************************************************
*
*	模块名称 : key驱动模块
*	文件名称 : bsp_key.c
*	版    本 : Vx.x
*	说    明 : 扫描独立按键，具有软件滤波机制，具有按键FIFO
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-03-08 waroyal  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "bsp.h"

///////////////////////////////////////
/* 变量定义区 */
static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;	//按键FIFO变量，结构体

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */
static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

///////////////////////////////////////
/* 函数实体区 */
/*复杂按键
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回1表示按下，0表示未按下
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
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键，该函数被bsp_Init()调用，需要声明
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();	//初始化按键变量
	bsp_InitKeyHard();	//初始化按键硬件
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将一个键值压入按键FIFO缓冲区，可用于模拟一个按键
*	形    参: 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_GetKey
*	功能说明: 从按键缓冲区中读取一个键值
*	形    参: 无
*	返 回 值: 按键代码
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
//*	函 数 名: bsp_GetKey2
//*	功能说明: 从按键缓冲区中读取一个键值
//*	形    参: 无
//*	返 回 值: 按键代码
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
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键状态
*	形    参: _ucKeyID：按键ID，从0开始
*	返 回 值: 1表示按下，0表示未按下
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参: _ucKeyID：按键ID，从0开始
				_LongTime：长按事件时间
				_RepeatSpeed：连发速度
*	返 回 值: 无
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
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read=s_tKey.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyhHard
*	功能说明: 配置按键相关的GPIO，该函数被 bsp_Init() 调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	//1.定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//2.打开时钟
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭jtag
	
	//3.结构体赋值
	GPIO_InitStructure.GPIO_Pin=WKUP_GPIO_PIN;//WKUP端口
	GPIO_InitStructure.GPIO_Mode=WKUP_GPIO_MODE;//WKUP端口模式
	GPIO_Init(WKUP_GPIO_PORT,&GPIO_InitStructure);//初始化WKUP
	
	GPIO_InitStructure.GPIO_Pin=KEY0_GPIO_PIN;//KEY0端口
	GPIO_InitStructure.GPIO_Mode=KEY0_GPIO_MODE;//KEY0端口模式
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure);//初始化KEY0
	
	GPIO_InitStructure.GPIO_Pin=KEY1_GPIO_PIN;//KEY1端口
	GPIO_InitStructure.GPIO_Mode=KEY1_GPIO_MODE;//KEY1端口模式
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);//初始化KEY1
	
	GPIO_InitStructure.GPIO_Pin=KEY2_GPIO_PIN;//KEY2端口
	GPIO_InitStructure.GPIO_Mode=KEY2_GPIO_MODE;//KEY2端口模式
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);//初始化KEY2	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;
	
	/*对按键FIFO读写指针清零*/
	s_tKey.Read=0;
	s_tKey.Write=0;
//	s_tKey.Read2=0;
	
	/*给每个按键结构体成员变量赋一组缺省值*/
	for(i=0;i<KEY_COUNT;i++)
	{
		s_tBtn[i].LongTime=KEY_LONG_TIME;	//长按时间0表示不检测长按键事件
		s_tBtn[i].Count=KEY_FILTER_TIME/2;	//计数器设置为滤波器时间的一半
		s_tBtn[i].State=0;					//按键缺省状态，0为未按下
		s_tBtn[i].RepeatSpeed=0;			//按键连发的速度，0表示不支持连发
		s_tBtn[i].RepeatCount=0;			//连发计数器
	}
	
	/*如果想要修改某个按键的参数，直接重定义*/
	//s_tBtn[KID_K1].LongTime=100;
	//s_tBtn[KID_K1].RepeatSpeed=5;	每隔50ms自动发送键值
	
	/*判断按键按下的函数*/
	s_tBtn[0].IsKeyDownFunc=IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc=IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc=IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc=IsKeyDown4;
	/*组合键*/
	s_tBtn[4].IsKeyDownFunc=IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc=IsKeyDown6;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键，非阻塞状态，必须被周期性的调用
*	形    参: 按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	
	pBtn=&s_tBtn[i];
	if(pBtn->IsKeyDownFunc())
	{
		//执行按键按下的处理
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
				//发送按钮按下的消息
				bsp_PutKey((uint8_t)(3*i+1));
			}
			
			if(pBtn->LongTime>0)
			{
				if(pBtn->LongCount<pBtn->LongTime)
				{
					//发送按钮连续按下的消息
					if(++pBtn->LongCount==pBtn->LongTime)
					{
						//键值放入按键FIFO
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
							//长按键后，每隔10ms发送一个按键
							bsp_PutKey((uint8_t)(3*i+1));
						}
					}
				}
			}
		}
	}
	else
	{
		//执行未按下按键或按键松手的处理
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
				//发送按键弹起的消息
				bsp_PutKey((uint8_t)(3*i+2));
			}
		}
		
		pBtn->LongCount=0;
		pBtn->RepeatCount=0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参: 无
*	返 回 值: 无
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

/*简单按键-------------------------------------------------------------------------------------------------------------------------------------------------------------------
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 按键输入
*	形    参: 无
*	返 回 值: WKUP_NUM、KEY0_NUM、KEY1_NUM、KEY2_NUM
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

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
