/*
*********************************************************************************************************
*
*	模块名称 : actuator_Data_Receive驱动模块
*	文件名称 : actuator_Data_Receive.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-18  waroyal  正式发布
*
*	Copyright (C), 2014-2024, 德致伦电子
*
*********************************************************************************************************
*/

//执行器模块上位机->下位机控制命令 功能码为0x01
/*
	USART_RX_BUF[]={							//串口1的接收缓存数组
		//帧头
		0x41,0x73,0x75,0x6b,0x61,		//Asuka 41 73 75 6B 61
		//数据区的长度
		0x05,												//len-FRAME_CONST_BYTE_LEN
		//数据区
		0x01,												//地址码 主卧-0x01 次卧-0x02 客厅-0x03
		0x01,												//继电器命令-0x01 or 风扇命令-0x02
		0x01,												//继电器号(0x01~0x04) or 风扇号(0x01~0x02) 
		0x01,												//翻转命令
		0x01,												//状态 开-0x00 关-0x01
		//校验位
		0x06												//数据区字节累加和
	};
*/

//执行器模块下位机->上位机发送反馈命令 功能码为0x02
/*
	XXXXX_RSP_BYTE[]={						//串口1的接收缓存数组
		//帧头
		0x41,0x73,0x75,0x6b,0x61,		//Asuka 41 73 75 6B 61
		//数据区的长度
		0x05,												//len-FRAME_CONST_BYTE_LEN
		//数据区
		0x01,												//地址码 主卧-0x01 次卧-0x02 客厅-0x03
		0x01,												//继电器命令-0x01 or 风扇命令-0x02
		0x01,												//继电器号(0x01~0x04) or 风扇号(0x01~0x02) 
		0x01,												//翻转命令
		0x01,												//状态 开-0x00 关-0x01
		//校验位
		0x07												//数据区字节累加和
	};
*/

///////////////////////////////////////
/* 头文件包含区 */
#include "uart1_cmd.h"

///////////////////////////////////////
/* 变量定义区 */
//设定传感器信息上报的缓存
uint8_t SENSOR_DATA_BYTE[248] ={
														FRAME_HEADER_BYTE_1,FRAME_HEADER_BYTE_2,FRAME_HEADER_BYTE_3,FRAME_HEADER_BYTE_4,FRAME_HEADER_BYTE_5,//帧头（5 个字节）
														0x51, //数据区长度（1 个字节）
														0x01, //类型为数据（1 个字节）
														0x01, //为传感器数据（1 个字节）
														//---- //传感器数据区（8*6 个字节）
														//- //校验（1 个字节）
														//- //地址码
														};
														
_sensorData Sensor_Data_ToFloat;//传感器数据转换为浮点数
_sensorData Sensor_Data_To4Byte;//传感器数据转换为 4 字节 内存

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
*	函 数 名: CheckSum
*	功能说明: 计算校验和
*	形    参：uint8_t USART_RX_BUF[]:数据区数据
*	返 回 值: 
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
*	函 数 名: actuator_Data_Receive
*	功能说明: 接收串口数据 根据数据控制执行器 并更新状态同时通过串口发送给上位机
*	形    参：void
*	返 回 值: void
*********************************************************************************************************
*/
void actuator_Data_Receive(void)
{
	u16 len = 0;//长度暂存
	
	if ( ReceiveState )//串口1接收到数据
	{
		len = RxCounter;
		
		//检验帧头
		if(USART_RX_BUF[0]==FRAME_HEADER_BYTE_1 && USART_RX_BUF[1]==FRAME_HEADER_BYTE_2 && USART_RX_BUF[2]==FRAME_HEADER_BYTE_3 && USART_RX_BUF[3]==FRAME_HEADER_BYTE_4 && USART_RX_BUF[4]==FRAME_HEADER_BYTE_5 )
		{
			//判断校验和是否正确
			if(USART_RX_BUF[len-1]==CheckSum(&USART_RX_BUF[FRAME_DATA_FIELD_OFFSET],len-FRAME_CONST_BYTE_LEN))
			{
				//判断数据长度是否正确
				if(USART_RX_BUF[5]==len-FRAME_CONST_BYTE_LEN)
				{
					//判断地址码 主卧-0x01
					if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET]==ADDRESS_ROOM)
					{
						//判断具体命令 翻转
						if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+3]==FUN_CODE_CMD_RELAY_TOGGLE)
						{
							//判断是继电器命令还是风扇命令
							switch(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+1])
							{
								case FUN_CODE_DEVICE_RELAY://继电器命令
								{
									//翻转继电器
									bsp_RelayToggle(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+2]);
								}break;
								case FUN_CODE_DEVICE_FAN://风扇命令
								{
									if(USART_RX_BUF[FRAME_DATA_FIELD_OFFSET+4]==0x00)//风扇状态为开
									{
										bsp_FanOn(1);
										bsp_FanOn(2);
									}else//风扇状态为关
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
		Uart_STA_Clr();//串口1状态清除
	}
}


/*
*********************************************************************************************************
* 函 数 名：sensor_TempHumi_Upload
* 功能说明：温湿度传感器字节协议上报函数
* 形    参：float temperature:温度浮点数数据
* 					float humidity:湿度浮点数数据
* 返 回 值：无
*********************************************************************************************************
*/ 
void sensor_Data_Upload(float temperature,float humidity,float co2,float pm25,float light)
{
	uint8_t i = 0;//循环数据
	//定义一下温度传感器
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
		
		
	//定义一下湿度传感器
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
	

	//定义一下co2传感器
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
	
	//定义一下pm2.5传感器
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
	
	//定义一下shock pir传感器
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
	
	//定义一下光照传感器
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

	//赋值，注意对共用体的的浮点数赋值
	Sensor_Data_To4Byte.f = temperature;
	//注意，内存中数组是小端模式存储的，因此需要转换为大端模式
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
	
	//偏移地址 0x05
	SENSOR_DATA_BYTE[DATA_LEN_OFFSET] = 51;//( 1 + 1 + 1 + 6 * 8 )长度 51 字节
	//偏移地址 0x06
	SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET] = FUN_CODE_DATA; //数据帧
	//偏移地址 0x07
	SENSOR_DATA_BYTE[DATA_TYPE_OFFSET] = DATA_TYPE_SENSOR; //传感器数据
	//偏移地址 0x08
	SENSOR_DATA_BYTE[SENSOR_DATA_LEN_OFFSET] = 48; //长度 48 个字节
	//偏移地址 0x09
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 0 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Temp_buf,8);//拷贝温度数据
	//偏移地址 0x11
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 1 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_Humi_buf,8);//拷贝湿度数据
	//偏移地址 0x19
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 2 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_co2_buf,8);//拷贝co2数据
	//偏移地址 0x21
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 3 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_pm25_buf,8);//拷贝PM2.5数据	
	//偏移地址 0x29
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 4 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_shock_pir_buf,8);//拷贝震动数据&红外数据
	//偏移地址 0x31
	memcpy( &SENSOR_DATA_BYTE[ ( SENSOR_DATA_START_OFFSET + ( 5 * SENSOR_SINGLE_DATA_LEN ) ) ],sensor_light_buf,8);//拷贝光照数据
	
	//计算校验和
	SENSOR_DATA_BYTE[( SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN ) )] = CheckSum( &SENSOR_DATA_BYTE[FRAME_TYPE_OFFSET],SENSOR_DATA_BYTE[DATA_LEN_OFFSET] );// 计算校验码;
	//地址码
	SENSOR_DATA_BYTE[( SENSOR_DATA_START_OFFSET + ( 6 * SENSOR_SINGLE_DATA_LEN ) )+1] = ADDRESS_ROOM;
	
	USART1_Send_Data(SENSOR_DATA_BYTE,59);//通过串口1发送传感器数据到主机
}

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
