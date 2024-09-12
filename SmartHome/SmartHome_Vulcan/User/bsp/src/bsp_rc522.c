/*
*********************************************************************************************************
*
*	模块名称 : rc522驱动模块
*	文件名称 : bsp_rc522.c
*	版    本 : Vx.x
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-06-28 waroyal  正式发布
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

///////////////////////////////////////
/* 外部变量申明区 */

///////////////////////////////////////
/* 函数申明区 */

///////////////////////////////////////
/* 函数实体区 */
/*
*********************************************************************************************************
* 函 数 名: bsp_InitRc522
* 功能说明: 初始化 RC522 的 IO 口和 SPI 功能配置
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitRc522(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_ALL_RC522, ENABLE ); //RC522 时钟使能
	
	GPIO_InitStructure.GPIO_Pin = RC522_CS_GPIO_PIN; // RC522_CS 推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RC522_CS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RC522_RST_GPIO_PIN; // RC522_RST 推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RC522_RST_GPIO_PORT, &GPIO_InitStructure);
	
	RC522_CS = 1; //RC522 不选中
	RC522_RST = 1; //RC522 不复位
	
	bsp_InitSPIx(RC522_SPIx); //初始化 CLK,MISO,MOSI 三个端口和 SPI 时钟
	/* SPI 模式配置 */
	// FLASH 芯片 支持 SPI 模式 0，据此设置 CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(RC522_SPIx , &SPI_InitStructure);
	/* 使能 SPI */
	SPI_Cmd(RC522_SPIx , ENABLE);
}

/*
*********************************************************************************************************
* 函 数 名: SPI_RC522_SendByte
* 功能说明: 从 SPI 总线写入一个字节
* 形 参: byte:要写入的字节
* 返 回 值: 返回接收到的数据
*********************************************************************************************************
*/
uint8_t SPI_RC522_SendByte(uint8_t byte)
{
 return SPIx_ReadWriteByte(RC522_SPIx,byte);
}

/*
*********************************************************************************************************
* 函 数 名: SPI_RC522_ReadByte
* 功能说明: 从 SPI 总线读取一个字节
* 形 参: 无
* 返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t SPI_RC522_ReadByte(void)
{
 return (SPI_RC522_SendByte(RC522_Dummy_Byte));
}

/*
*********************************************************************************************************
* 函 数 名: ReadRawRC
* 功能说明: 读 RC522 寄存器
* 形 参: Address:寄存器地址
* 返 回 值: 读出的值
*********************************************************************************************************
*/
uint8_t ReadRawRC(uint8_t Address)
{
	uint8_t ucAddr;
	uint8_t ucResult=0;
	
	ucAddr = ( ( ( Address <<1 ) & 0x7E ) | 0x80 );
	
	/*选择 RC522: CS 低 */
	/* 通讯开始：CS 低 */
	RC522_CS = 0;
	SPI_RC522_SendByte( ucAddr );
	ucResult = SPI_RC522_ReadByte();
	/* 停止信号 RC522: CS 高 */
	/* 通讯开始：CS 低 */
	RC522_CS = 1;
	return ucResult;
}

/*
*********************************************************************************************************
* 函 数 名: ReadRawRC
* 功能说明: 写 RC522 寄存器
* 形 参: uint8_t Address:寄存器地址
* uint8_t value:写入的值
* 返 回 值: 无
*********************************************************************************************************
*/
void WriteRawRC(uint8_t Address, uint8_t value)
{ 
	uint8_t ucAddr;
	ucAddr = ( ( Address << 1 ) & 0x7E );
	/*选择 RC522: CS 低 */
	/* 通讯开始：CS 低 */
	RC522_CS = 0;
	SPI_RC522_SendByte( ucAddr );
	SPI_RC522_SendByte( value );
	/* 停止信号 RC522: CS 高 */
	/* 通讯开始：CS 低 */
	RC522_CS = 1;
}

/*
*********************************************************************************************************
* 函 数 名: SetBitMask
* 功能说明: 对 RC522 寄存器置位
* 形 参: uint8_t ucReg，寄存器地址
* uint8_t ucMask，置位值
* 返 回 值: 无
*********************************************************************************************************
*/
void SetBitMask ( uint8_t ucReg, uint8_t ucMask ) 
{
  uint8_t ucTemp = 0;
  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp | ucMask ); // set bit mask
}

/*
*********************************************************************************************************
* 函 数 名: ClearBitMask
* 功能说明: 对 RC522 寄存器清位
* 形 参: uint8_t ucReg，寄存器地址
* uint8_t ucMask，清位值
* 返 回 值: 无
*********************************************************************************************************
*/
void ClearBitMask ( uint8_t ucReg, uint8_t ucMask ) 
{
  uint8_t ucTemp = 0;
  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); // clear bit mask
}

/*
*********************************************************************************************************
* 函 数 名: PcdAntennaOn
* 功能说明: 开启天线
* 形 参: 无
* 返 回 值: 无
* 注 意：每次启动或关闭天险发射之间应至少有 1ms 的间隔
*********************************************************************************************************
*/
void PcdAntennaOn ( void )
{
  uint8_t uc;
  uc = ReadRawRC ( TxControlReg );
  if ( ! ( uc & 0x03 ) )
	{
    SetBitMask( TxControlReg, 0x03 );
  }
}

/*
*********************************************************************************************************
* 函 数 名: PcdAntennaOff
* 功能说明: 关闭天线
* 形 参: 无
* 返 回 值: 无
* 注 意：每次启动或关闭天险发射之间应至少有 1ms 的间隔
*********************************************************************************************************
*/
void PcdAntennaOff ( void )
{
	ClearBitMask ( TxControlReg, 0x03 );
}

/*
*********************************************************************************************************
* 函 数 名: PcdReset
* 功能说明: 复位 RC522 
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
void PcdReset ( void )
{
	/* 复位 RC522: RST 高 */
	RC522_RST = 1;
	delay_us(10); //_NOP();
	/* 取消复位 RC522: RST 低 */
	RC522_RST = 0;
	delay_us(10); //_NOP();
	/* 复位 RC522: RST 高 */
	RC522_RST = 1;
	delay_us(10); //_NOP();
	WriteRawRC ( CommandReg, 0x0f );
	while ( ReadRawRC ( CommandReg ) & 0x10 );
	delay_us(10); //_NOP();
	WriteRawRC ( ModeReg, 0x3D ); //定义发送和接收常用模式 和 Mifare 卡通讯，CRC 初始值 0x6363
	WriteRawRC ( TReloadRegL, 30 ); //16 位定时器低位 
	WriteRawRC ( TReloadRegH, 0 ); //16 位定时器高位
	WriteRawRC ( TModeReg, 0x8D ); //定义内部定时器的设置
	WriteRawRC ( TPrescalerReg, 0x3E ); //设置定时器分频系数
	WriteRawRC ( TxAutoReg, 0x40 ); //调制发送信号为 100%ASK
}

/*
*********************************************************************************************************
* 函 数 名: PcdComMF522
* 功能说明: 通过 RC522 和 ISO14443 卡通讯
* 形 参: ucCommand，RC522 命令字
* pInData，通过 RC522 发送到卡片的数据
* ucInLenByte，发送数据的字节长度
* pOutData，接收到的卡片返回数据
* pOutLenBit，返回数据的位长度
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdComMF522 ( uint8_t ucCommand, uint8_t * pInData, uint8_t ucInLenByte, uint8_t * pOutData, uint32_t * pOutLenBit )
{
  char cStatus = MI_ERR;
  uint8_t ucIrqEn = 0x00;
  uint8_t ucWaitFor = 0x00;
  uint8_t ucLastBits;
  uint8_t ucN;
  uint32_t ul;
  switch ( ucCommand )
  {
    case PCD_AUTHENT: //Mifare 认证
    {
			ucIrqEn = 0x12; //允许错误中断请求 ErrIEn 允许空闲中断 IdleIEn
			ucWaitFor = 0x10; //认证寻卡等待时候 查询空闲中断标志位
		}break;
		case PCD_TRANSCEIVE: //接收发送 发送接收
		{
			ucIrqEn = 0x77; //允许 TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
			ucWaitFor = 0x30; //寻卡等待时候 查询接收中断标志位与 空闲中断标志位
		}break;
		default:break;
	}
	 
	 WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 ); //IRqInv 置位管脚 IRQ 与 Status1Reg 的 IRq 位的值相反
	 ClearBitMask ( ComIrqReg, 0x80 ); //Set1 该位清零时，CommIRqReg 的屏蔽位清零
	 WriteRawRC ( CommandReg, PCD_IDLE ); //写空闲命令
	 SetBitMask ( FIFOLevelReg, 0x80 ); //置位 FlushBuffer 清除内部 FIFO 的读和写指针以及 ErrReg 的BufferOvfl 标志位被清除
	 
	 for ( ul = 0; ul < ucInLenByte; ul ++ )
	{
		WriteRawRC ( FIFODataReg, pInData [ ul ] ); //写数据进 FIFOdata
	}
	 WriteRawRC ( CommandReg, ucCommand ); //写命令
	 
	 if ( ucCommand == PCD_TRANSCEIVE )
	{
		SetBitMask(BitFramingReg,0x80); //StartSend 置位启动数据发送 该位与收发命令使用时才有效
	}
	 
	 ul = 1000;//根据时钟频率调整，操作 M1 卡最大等待时间 25ms
	 do //认证 与寻卡等待时间
	 {
			ucN = ReadRawRC ( ComIrqReg ); //查询事件中断
			ul --;
	 } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) ); //退出条件 i=0,定时器中断，与写空闲命令
	 ClearBitMask ( BitFramingReg, 0x80 ); //清理允许 StartSend 位
	 if ( ul != 0 )
	{
		if ( ! ( ReadRawRC ( ErrorReg ) & 0x1B ) ) // 读 错 误 标 志 寄 存 器 BufferOfI CollErr ParityErr ProtocolErr
		{
			 cStatus = MI_OK;
			 if ( ucN & ucIrqEn & 0x01 ) //是否发生定时器中断
			 {
         cStatus = MI_NOTAGERR;
       }
      if ( ucCommand == PCD_TRANSCEIVE )
      {
				  ucN = ReadRawRC ( FIFOLevelReg ); //读 FIFO 中保存的字节数
				  ucLastBits = ReadRawRC ( ControlReg ) & 0x07; //最后接收到得字节的有效位数
					if ( ucLastBits )
					{
						* pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits; //N 个字节数减去 1（最后一个字节）+最后一位的位数 读取到的数据总位数
					}
					else
					{
						* pOutLenBit = ucN * 8; //最后接收到的字节整个字节有效
					}
					if ( ucN == 0 )
					{
						ucN = 1; 
					}
					if ( ucN > MAXRLEN )
					{
					 ucN = MAXRLEN; 
					}
					for ( ul = 0; ul < ucN; ul ++ )
					{
						pOutData [ ul ] = ReadRawRC ( FIFODataReg ); 
					}
			}
	}		
	else
	{
		cStatus = MI_ERR; 
	}
 
 }
 
	SetBitMask ( ControlReg, 0x80 ); // stop timer now
	WriteRawRC ( CommandReg, PCD_IDLE ); 
	return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: M500PcdConfigISOType
* 功能说明: 设置 RC522 的工作方式
* 形 参: ucType，工作方式
* 返 回 值: 无
*********************************************************************************************************
*/
void M500PcdConfigISOType ( uint8_t ucType )
{
	if ( ucType == 'A') //ISO14443_A
	{
		ClearBitMask ( Status2Reg, 0x08 );
		WriteRawRC ( ModeReg, 0x3D ); //3F
		WriteRawRC ( RxSelReg, 0x86 ); //84
		WriteRawRC( RFCfgReg, 0x7F ); //4F
		WriteRawRC( TReloadRegL, 30 );//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC ( TReloadRegH, 0 );
		WriteRawRC ( TModeReg, 0x8D );
		WriteRawRC ( TPrescalerReg, 0x3E );
		delay_us ( 2 );
		PcdAntennaOn ();//开天线
	}
}

/*
*********************************************************************************************************
* 函 数 名: PcdRequest
* 功能说明: 寻卡
* 形 参: ucReq_code，寻卡方式
* = 0x52，寻感应区内所有符合 14443A 标准的卡
* = 0x26，寻未进入休眠状态的卡
* pTagType，卡片类型代码
* = 0x4400，Mifare_UltraLight
* = 0x0400，Mifare_One(S50)
* = 0x0200，Mifare_One(S70)
* = 0x0800，Mifare_Pro(X))
* = 0x4403，Mifare_DESFire
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdRequest ( uint8_t ucReq_code, uint8_t * pTagType )
{
	char cStatus; 
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t ulLen;
	
	ClearBitMask ( Status2Reg, 0x08 ); //清理指示 MIFARECyptol 单元接通以及所有卡的数据通信被加密的情况
	WriteRawRC ( BitFramingReg, 0x07 ); // 发送的最后一个字节的 七位
	SetBitMask ( TxControlReg, 0x03 ); //TX1,TX2 管脚的输出信号传递经发送调制的 13.56 的能量载波信号
	ucComMF522Buf [ 0 ] = ucReq_code; //存入 卡片命令字
	cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, & ulLen ); //寻卡 
	if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) ) //寻卡成功返回卡类型
	{ 
		*pTagType = ucComMF522Buf [ 0 ];
		*( pTagType + 1 ) = ucComMF522Buf [ 1 ];
	}
	else
	{
		cStatus = MI_ERR;
	}
	return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: PcdAnticoll
* 功能说明: 防冲撞
* 形 参: pSnr，卡片序列号，4 字节
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdAnticoll ( uint8_t * pSnr )
{
	char cStatus;
	uint8_t uc, ucSnr_check = 0;
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t ulLen;
	
	ClearBitMask ( Status2Reg, 0x08 ); //清 MFCryptol On 位 只有成功执行 MFAuthent 命令后，该位才能置位
	WriteRawRC ( BitFramingReg, 0x00); //清理寄存器 停止收发
	ClearBitMask ( CollReg, 0x80 ); //清 ValuesAfterColl 所有接收的位在冲突后被清除
	
	ucComMF522Buf [ 0 ] = 0x93; //卡片防冲突命令
	ucComMF522Buf [ 1 ] = 0x20;
	
	cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);//与卡片通信
	if ( cStatus == MI_OK) //通信成功
	{
		for ( uc = 0; uc < 4; uc ++ )
		{
			*( pSnr + uc ) = ucComMF522Buf [ uc ]; //读出 UID
			ucSnr_check ^= ucComMF522Buf [ uc ];
		}
		if ( ucSnr_check != ucComMF522Buf [ uc ] )
		{
			cStatus = MI_ERR; 
		}
	}
	SetBitMask ( CollReg, 0x80 );
	return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: CalulateCRC
* 功能说明: 用 RC522 计算 CRC16 
* 形 参: 
* pIndata，计算 CRC16 的数组
* ucLen，计算 CRC16 的数组字节长度
* pOutData，存放计算结果存放的首地址
* 返 回 值: 无
*********************************************************************************************************
*/
void CalulateCRC ( uint8_t * pIndata, uint8_t ucLen, uint8_t * pOutData )
{
	 uint8_t uc, ucN;
	 ClearBitMask(DivIrqReg,0x04);
	 WriteRawRC(CommandReg,PCD_IDLE);
	 SetBitMask(FIFOLevelReg,0x80);
	 for ( uc = 0; uc < ucLen; uc ++)
	{
		WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );
	}
	 WriteRawRC ( CommandReg, PCD_CALCCRC );
	 uc = 0xFF;
	 do 
	 {
		ucN = ReadRawRC ( DivIrqReg );
		uc --;
	 } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
	  pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
		pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );
}

/*
*********************************************************************************************************
* 函 数 名: PcdSelect
* 功能说明: 选定卡片
* 形 参: pSnr，卡片序列号，4 字节
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdSelect ( uint8_t * pSnr )
{
	char ucN;
	uint8_t uc;
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t ulLen;
	
	ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
	ucComMF522Buf [ 1 ] = 0x70;
	ucComMF522Buf [ 6 ] = 0;
	
	for ( uc = 0; uc < 4; uc ++ )
	{
		ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
		ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
	}
	CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
	ClearBitMask ( Status2Reg, 0x08 );
	
	ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
	if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
	{
		ucN = MI_OK;
	}
	else
	{
		ucN = MI_ERR; 
	}
	return ucN;
}

/*
*********************************************************************************************************
* 函 数 名: PcdSelect
* 功能说明: 验证卡片密码
* 形 参: 
* ucAuth_mode，密码验证模式
* = 0x60，验证 A 密钥
* = 0x61，验证 B 密钥
* ucAddr，块地址
* pKey，密码
* pSnr，卡片序列号，4 字节
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdAuthState ( uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t * pKey, uint8_t * pSnr )
{
	char cStatus;
	uint8_t uc, ucComMF522Buf [ MAXRLEN ];
	uint32_t ulLen;
	 
	 ucComMF522Buf [ 0 ] = ucAuth_mode;
	 ucComMF522Buf [ 1 ] = ucAddr;
  for ( uc = 0; uc < 6; uc ++ )
	{
		ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );
	} 
	for ( uc = 0; uc < 6; uc ++ )
	{	
		ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );
	}
	cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
  if ( ( cStatus != MI_OK ) || ( ! ( ReadRawRC ( Status2Reg ) & 0x08 ) ) )
	{
		cStatus = MI_ERR;
	}
  return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: PcdWrite
* 功能说明: 写数据到 M1 卡一块
* 形 参: 
* ucAddr，块地址
* pData，写入的数据，16 字节
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdWrite ( uint8_t ucAddr, uint8_t * pData )
{
	char cStatus;
	uint8_t uc, ucComMF522Buf [ MAXRLEN ];
	uint32_t ulLen;
	
	ucComMF522Buf [ 0 ] = PICC_WRITE;
	ucComMF522Buf [ 1 ] = ucAddr;
	
	CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
	cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
	{
		cStatus = MI_ERR; 
	}
	if ( cStatus == MI_OK )
	{
		//memcpy(ucComMF522Buf, pData, 16);
		for ( uc = 0; uc < 16; uc ++ )
		{
		ucComMF522Buf [ uc ] = * ( pData + uc ); 
		}
		CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );
		cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
		if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
		{
		cStatus = MI_ERR;
		}
	} 
	return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: PcdRead
* 功能说明: 读取 M1 卡一块数据
* 形 参: 
* ucAddr，块地址
* pData，读出的数据，16 字节
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdRead ( uint8_t ucAddr, uint8_t * pData )
{
	char cStatus;
	uint8_t uc, ucComMF522Buf [ MAXRLEN ]; 
	uint32_t ulLen;
	
	ucComMF522Buf [ 0 ] = PICC_READ;
	ucComMF522Buf [ 1 ] = ucAddr;
	
	CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
	
	cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
	{
		for ( uc = 0; uc < 16; uc ++ )
		* ( pData + uc ) = ucComMF522Buf [ uc ]; 
	}
	else
	{
		cStatus = MI_ERR; 
	}
	 
	return cStatus;
}

/*
*********************************************************************************************************
* 函 数 名: PcdHalt
* 功能说明: 命令卡片进入休眠状态
* 形 参: 无
* 返 回 值: 状态值= MI_OK，成功
*********************************************************************************************************
*/
char PcdHalt( void )
{
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t ulLen;
	
	ucComMF522Buf [ 0 ] = PICC_HALT;
	ucComMF522Buf [ 1 ] = 0;
	
	CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
	PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	
	return MI_OK;
}

///////////////////////////////////////

/***************************** 德致伦电子 DeZLinc (END OF FILE) *********************************/
