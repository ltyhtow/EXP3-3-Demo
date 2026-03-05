#include "iic_oled.h"

unsigned char  ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};//定义屏幕的存储空间

/**
 * ************************************************************************
 * @brief	OLED_SDA引脚设置推挽输出模式
 * 
 * ************************************************************************
 */
void OLED_SDA_Out(void)
{					     
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = OLED_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(OLED_SDA_GPIO_Port, &GPIO_InitStruct);
}

/**
 * ************************************************************************
 * @brief	OLED_SDA引脚设置上拉模式
 * 
 * ************************************************************************
 */
void OLED_SDA_In(void)
{		
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = OLED_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(OLED_SDA_GPIO_Port, &GPIO_InitStruct);
}

/**
 * ************************************************************************
 * @brief	启动OLED的IIC通信
 * 
 * ************************************************************************
 */
void OLED_IIC_Start(void)
{
	OLED_SCLK_SET_1;	//SCLK线置高
	OLED_SDA_SET_1;		//SDA线置高
	OLED_SDA_SET_0;		//SDA线置低
	OLED_SCLK_SET_0;	//SCLK线置低
}
 
/**
 * ************************************************************************
 * @brief	停止OLED的IIC通信
 * 
 * ************************************************************************
 */
void OLED_IIC_Stop(void)
{
	OLED_SCLK_SET_0;	//SCLK线置低
	OLED_SDA_SET_0;		//SDA线置低
	OLED_SCLK_SET_1;	//SCLK线置高
	OLED_SDA_SET_1;		//SDA线置高
}

/**
 * ************************************************************************
 * @brief	等待从机的应答函数
 * 
 * @retval unsigned char 1：接收应答失败；0：接收应答成功。
 * 
 * ************************************************************************
 */
unsigned char OLED_IIC_WaitAck(void) 
{
	unsigned char timeout = 0;
	OLED_SDA_In();						//SDA设置输入模式
	OLED_SCLK_SET_1;					//SCLK线置高
	while(OLED_SDA_GET)					//等待从机拉低SDA线。
	{
		timeout++;
		if(timeout > 250)				//等待超时处理
		{
			OLED_IIC_Stop();			//停止OLED的IIC通信
			return 1;					//返回参数1,接收应答失败
		}
	}
	OLED_SCLK_SET_0;					//SCLK线置低
	OLED_SDA_Out();						//SDA设置输出模式
	return 0;
}

/**
 * ************************************************************************
 * @brief	通过IIC协议向OLED写入一个字节的数据
 * 
 * @param   Data	IIC写入的数据
 * ************************************************************************
 */
void OLED_IIC_WriteByte(unsigned char Data)
{
	unsigned char i;
	OLED_SCLK_SET_0;					//SCLK线置低
	for( i = 0 ; i < 8 ; i ++)			//循环8次，每次写入最高一位数据		
	{
		if((Data & 0x80) == 0x80)		//写入最高一位数据
		{
			OLED_SDA_SET_1;
		}
		else
		{
			OLED_SDA_SET_0;
		}
		Data = Data << 1;				//数据向高位移动一位
		OLED_SCLK_SET_1;				//SCLK线置高	
		OLED_SCLK_SET_0;				//SCLK线置低
	}
}

/**
 * ************************************************************************
 * @brief	通过IIC协议向OLED发送命令的函数
 * 
 * @param   Cmd		写入的命令
 * ************************************************************************
 */
void OLED_WriteCmd(unsigned char Cmd)
{
	OLED_IIC_Start();						//启动IIC通信
	OLED_IIC_WriteByte(OLED_ADDRESS);		//写入OLED的IIC地址
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_WriteByte(0x00);				//写入控制字节，表示接下来的数据是命令
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_WriteByte(Cmd); 				//写入具体的命令字节
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_Stop();						//停止IIC通信
	
}

/**
 * ************************************************************************
 * @brief	通过IIC协议向OLED发送数据的函数
 * 
 * @param   Data		写入的数据
 * ************************************************************************
 */
void OLED_WriteData(unsigned char Data)
{
	OLED_IIC_Start();						//启动IIC通信
	OLED_IIC_WriteByte(OLED_ADDRESS);		//写入OLED的IIC地址
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_WriteByte(0x40);				//写入控制字节，表示后续数据为数据字节
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_WriteByte(Data);				//写入实际的数
	if(OLED_IIC_WaitAck())return;			//等待从机应答失败，返回
	OLED_IIC_Stop();						//停止IIC通信
}

/**
 * ************************************************************************
 * @brief	开启OLED
 * 
 * ************************************************************************
 */
void OLED_DisplayOn(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X14);  //开启电荷泵
	OLED_WriteCmd(0XAF);  //OLED唤醒
}

/**
 * ************************************************************************
 * @brief	休眠OLED
 * 
 * ************************************************************************
 */
void OLED_DisplayOff(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X10);  //关闭电荷泵
	OLED_WriteCmd(0XAE);  //OLED休眠
}

/**
 * ************************************************************************
 * @brief	清除OLED屏幕上的所有内容
 * 
 * ************************************************************************
 */
void OLED_Clear(void)
{
	unsigned char page,column;// 初始化两个无符号字符变量
	for( page = 0 ; page < 8 ; page ++)
	{
		OLED_WriteCmd(0xB0 + page);	// 向OLED写入命令以设置页地址
		OLED_WriteCmd(0x00);		// 向OLED写入命令以设置低位列地址
		OLED_WriteCmd(0x10);		// 向OLED写入命令以设置高位列地址
		for( column = 0 ; column < 128 ; column ++)
		{
			// 向OLED写入数据，此处数据为0x00，表示清空该位置的显示内容
			OLED_WriteData(0x00);
		}
	}
}

/**
 * ************************************************************************
 * @brief	刷新OLED整个RAM的函数（将ScreenBuffer中的数据写入OLED显示RAM，以更新显示内容）
 * 
 * ************************************************************************
 */
void OLED_RefreshRAM(void)
{
	unsigned char page,column;// 初始化两个无符号字符变量
	for(page = 0; page < SCREEN_ROW/8; page++)// 页寻址模式填充
	{
		OLED_WriteCmd(0xB0+page);			//设置页地址b0~b7
		OLED_WriteCmd(0x00);				//设置显示位置—列低地址00-0f
		OLED_WriteCmd(0x10);				//设置显示位置—列高地址10-1f
		for(column = 0; column < SCREEN_COLUMN; column++)
		{
			OLED_WriteData(ScreenBuffer[page][column]);//向OLED的RAM中填写ScreenBuffer中的数据
		}
	} 
}

/**
 * ************************************************************************
 * @brief 	刷新OLED部分区域的RAM的函数（将ScreenBuffer中的数据写入OLED显示RAM，以更新显示内容）
 * 
 * @param     x1  			起始点横坐标(x:0~127）
 * @param     y1 			起始点纵坐标(y:0~63）
 * @param     x2  			起始点横坐标(x:0~127）
 * @param     y2 			起始点纵坐标(y:0~63）
 * 
 * ************************************************************************
 */
void OLED_RefreshAreaRAM(unsigned char x1, unsigned char y1 ,unsigned char x2, unsigned char y2) 
{ 	
	//范围限制最大不超过液晶的大小
	if( x2 >= SCREEN_COLUMN) x2 = SCREEN_COLUMN-1;
	if( y2 >= SCREEN_ROW) y2 = SCREEN_ROW-1;
	//限制x1,y1坐标小于等于x2,y2坐标
	if (x2 >= x1 && x2 < SCREEN_COLUMN && y2 >= y1 && y2 < SCREEN_ROW)
	{
		unsigned char page,column;// 初始化两个无符号字符变量
		for(page = y1/8; page < y2/8+1; page++)// 页寻址模式填充
		{
			OLED_WriteCmd(0xB0+page);						//设置页地址b0~b7
			OLED_WriteCmd( x1 & 0x0F); 						//设置显示位置—列低地址00-0f
			OLED_WriteCmd( ( (x1 & 0xF0) >> 4) | 0X10);		//设置显示位置—列高地址10-1f
			for(column = x1; column < x2+1; column++)
			{
				OLED_WriteData(ScreenBuffer[page][column]);//向OLED的RAM中填写ScreenBuffer中的数据
			}
		}
	}	
}

/**
 * ************************************************************************
 * @brief 	清除ScreenBuffer中数据和OLED数据
 * 
 * 
 * ************************************************************************
 */
void OLED_ClearRAM(void)
{
	unsigned char page,column;// 初始化两个无符号字符变量
	for(page = 0; page < SCREEN_ROW/8; page++)// 页寻址模式填充
	{
		for(column = 0; column < SCREEN_COLUMN; column++)
		{
			ScreenBuffer[page][column] = 0x00;//清除除ScreenBuffer中数据
		}
	}
	OLED_RefreshRAM(); //刷新OLED整个RAM的函数
}


/**
 * ************************************************************************
 * @brief 屏幕内容取反显示
 * 
 * @param     Mode  开关
 * 					开	ON	0xA7	点亮全屏
 *  				关	OFF	0xA6	默认此模式，设置像素点亮
 * 
 * ************************************************************************
 */
void OLED_DisplayMode(unsigned char Mode)
{
	OLED_WriteCmd(Mode);
}

/**
 * ************************************************************************
 * @brief 屏幕亮度调节
 * 
 * @param     Intensity  亮度大小（0~255），默认为0x7f
 * 
 * ************************************************************************
 */
void OLED_IntensityControl(unsigned char Intensity)
{
	OLED_WriteCmd(0X81);
	OLED_WriteCmd(Intensity);
}

/**
 * ************************************************************************
 * @brief 全屏内容偏移指定距离
 * 
 * @param     ShiftNum  偏移距离（0~63）
 * 
 * ************************************************************************
 */
void OLED_Shift(unsigned char ShiftNum)
{
	for(unsigned char i = 0; i < ShiftNum; i++)
	{
		OLED_WriteCmd(0xD3);//设置显示偏移，垂直向上偏移
		OLED_WriteCmd(i);//偏移量
		OLED_DelayMs(10);//延时时间
	}
}

/**
 * ************************************************************************
 * @brief 屏幕内容水平方向滚动播放
 * 
 * @param     StartPage  	开始页数	(0~7)
 * @param     EndPage  		结束页数	(0~7)
 * @param     Direction  	滚动方向
 * 								左		LEFT	0x27
 * 								右		RIGHT	0x26
 * @note 在开始页数和结束页数之间的内容才会滚动播放，且写命令顺序不得改变
 * ************************************************************************
 */
void OLED_HorizontalShift(unsigned char StartPage,unsigned char EndPage,unsigned char Direction)
{
	OLED_WriteCmd(0x2E);  			//关闭滚动

	OLED_WriteCmd(Direction);		//设置滚动方向
	OLED_WriteCmd(0x00);			//虚拟字节设置，默认为0x00
	OLED_WriteCmd(StartPage);		//设置开始页地址
	OLED_WriteCmd(0x05);			//设置每个滚动步骤之间的时间间隔的帧频
	OLED_WriteCmd(EndPage);			//设置结束页地址
	OLED_WriteCmd(0x00);			//虚拟字节设置，默认为0x00
	OLED_WriteCmd(0xFF);			//虚拟字节设置，默认为0xff

	OLED_WriteCmd(0x2F);			//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}


/**
 * ************************************************************************
 * @brief 设置坐标像素点数据
 * 
 * @param     x  			起始点横坐标(x:0~127）
 * @param     y  			起始点纵坐标(y:0~63）
 * @param     DrawPoint  	设置该点的数据 DrawPoint = 1, 清除该点的数据 DrawPoint = 0
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char DrawPoint , unsigned int RefreshDisplay)
{ 
	if (x < SCREEN_COLUMN && y < SCREEN_ROW ) 
	{
		
		if( ( (ScreenBuffer[y/8][x] >> (y % 8) ) & 0x01 ) != DrawPoint)
		{
			if(DrawPoint)
			{
				ScreenBuffer[y/8][x] |= (0x01 << (y % 8));
			}  
			else
			{
				ScreenBuffer[y/8][x] &= ~(0x01 << (y % 8));
			}
			if(RefreshDisplay == 1)
			{
				OLED_RefreshAreaRAM(x, y, x, y);
			}
		}
	}
}

/**
 * ************************************************************************
 * @brief 绘制一条x1,y1到x2,y2的线
 * 
 * @param     x1  			起始点横坐标(x:0~127）
 * @param     y1  			起始点纵坐标(y:0~63）
 * @param     x2  			结束点横坐标(x:0~127）
 * @param     y2  			结束点纵坐标(y:0~63）
 * @param     DrawLine  	设置的数据 DrawLine = 1, 清除的数数据 DrawLine = 0
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_DrawLine(unsigned char x1 , unsigned char y1 , unsigned char x2 , unsigned char y2 , unsigned char DrawLine , unsigned int RefreshDisplay)
{
	unsigned int t; 
	int xerr=0,yerr=0,deltax,deltay,distance;
	int incx,incy,urow,ucol;
	deltax = x2-x1; 			//计算坐标增量 
	deltay = y2-y1;
	urow = x1;				//画线起点坐标
	ucol = y1;
	if(deltax > 0)
	{
		incx = 1; 			
	}
	else if(deltax==0)
	{
		incx = 0;		
	}
	else 
	{
		incx = -1;
		deltax = -deltax;
	}
	
	if(deltay>0)
	{
		incy = 1;
	}
	else if(deltay == 0)
	{
		incy = 0;
	}
	else 
	{
		incy = -1;
		deltay = -deltay;
	}
	
	if(deltax>deltay)
	{
		distance = deltax; 
	}
	else 
	{
		distance = deltay;
	}
	
	for(t = 0 ; t < distance+1 ; t++)
	{
		OLED_DrawPoint(urow,ucol,DrawLine,RefreshDisplay);
		xerr += deltax;
		yerr += deltay;
		if(xerr > distance)
		{
			xerr -= distance;
			urow += incx;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			ucol += incy;
		}
	}
}

/**
 * ************************************************************************
 * @brief 绘制矩形
 * 
 * @param     x1  			起始点横坐标(x:0~127）
 * @param     y1  			起始点纵坐标(y:0~63）
 * @param     x2  			结束点横坐标(x:0~127）
 * @param     y2  			结束点纵坐标(y:0~63）
 * @param     DrawRectangle  	设置的数据 DrawRectangle = 1, 清除的数数据 DrawRectangle = 0
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */

void OLED_DrawRectangle(unsigned char x1 , unsigned char y1 , unsigned char x2 , unsigned char y2 , unsigned char DrawRectangle , unsigned int RefreshDisplay)

{
	OLED_DrawLine(x1,y1,x2,y1,DrawRectangle,RefreshDisplay);
	OLED_DrawLine(x1,y1,x1,y2,DrawRectangle,RefreshDisplay);
	OLED_DrawLine(x1,y2,x2,y2,DrawRectangle,RefreshDisplay);
	OLED_DrawLine(x2,y1,x2,y2,DrawRectangle,RefreshDisplay);
}


/**
 * ************************************************************************
 * @brief 绘制画一个指定大小的圆
 * 
 * @param     x  			中心点横坐标(x:0~127）
 * @param     y 			中心点纵坐标(y:0~63）
 * @param     Radius  		半径
 * @param     DrawCircle  	设置的数据 DrawCircle = 1, 清除的数数据 DrawCircle = 0
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void LCD_DrawCircle(unsigned char x,unsigned char y,unsigned char Radius , unsigned char DrawCircle , unsigned int RefreshDisplay)
{
	int a,b;
	int di;
	a = 0;b = Radius;	  
	di = 3-(Radius << 1);             //判断下个点位置的标志
	while(a <= b)
	{
		OLED_DrawPoint(x+a,y-b,DrawCircle,RefreshDisplay);            //5
 		OLED_DrawPoint(x+b,y-a,DrawCircle,RefreshDisplay);            //0           
		OLED_DrawPoint(x+b,y+a,DrawCircle,RefreshDisplay);            //4               
		OLED_DrawPoint(x+a,y+b,DrawCircle,RefreshDisplay);            //6 
		OLED_DrawPoint(x-a,y+b,DrawCircle,RefreshDisplay);            //1       
 		OLED_DrawPoint(x-b,y+a,DrawCircle,RefreshDisplay);            
		OLED_DrawPoint(x-a,y-b,DrawCircle,RefreshDisplay);            //2             
  		OLED_DrawPoint(x-b,y-a,DrawCircle,RefreshDisplay);            //7     	         
		a ++;
		//使用Bresenham算法画圆     
		if(di < 0)di += 4*a+6;	  
		else
		{
			di += 10+4*(a-b);   
			b--;
		} 						    
	}
} 


/**
 * ************************************************************************
 * @brief 字符显示函数
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     Data  字符
 * @param     Size  字符大小 8,12,16,24
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Data,unsigned char Size , unsigned int RefreshDisplay)
{      	
	if (x < SCREEN_COLUMN && y < SCREEN_ROW ) 
	{
		int32_t position = 0;
		unsigned char page,line;
		unsigned char temp;
		position = Data - ' ';			//计算ascii对应的数组位置
		if(position < 0)return;			//无效字符直接返回
		switch(Size)					//获取字符占用页，占用列
		{
			#if FONT8 
				case FSIZE8_6   :	page = 1 ; line = 6  ;break; 
			#endif
			#if FONT12
				case FSIZE12_6  :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
			#if FONT16
				case FSIZE16_8  :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
			#if FONT24
				case FSIZE24_12 :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
		}
		for(unsigned char m = 0; m < page; m++)
		{
			for(unsigned char n = 0; n < line; n++)
			{
				switch(Size)
				{
					#if FONT8 
						case FSIZE8_6   :	temp = F8X6  [position][n+m*line] ;break;
					#endif
					#if FONT12
						case FSIZE12_6  :	temp = F12X6 [position][n+m*line] ;break;
					#endif
					#if FONT16
						case FSIZE16_8  :	temp = F16X8 [position][n+m*line] ;break;
					#endif
					#if FONT24
						case FSIZE24_12 :	temp = F24X12[position][n+m*line] ;break;
					#endif
				}
				for(unsigned char i = 0; i < 8; i++)
				{
					OLED_DrawPoint(x+n, y+i+m*8, (temp >> i) & 0x01 , 0);
				}
			}	
		}
		if(RefreshDisplay == 1)
		{
			OLED_RefreshAreaRAM(x, y, x+line, y+8*page);
		}
	}
}


/**
 * ************************************************************************
 * @brief 字符串显示函数
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     Data   字符串（通过双引号引入）
 * @param     Size  字符大小 8,12,16,24
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *Data, unsigned char Size , unsigned int RefreshDisplay)
{ 
	if (x < SCREEN_COLUMN && y < SCREEN_ROW ) 
	{
		unsigned int Strdatalenght= strlen((char *)Data);
		unsigned int strcount = 0;
		unsigned char page,line;
		switch(Size)					//获取字符占用页，占用列
		{
			#if FONT8 
				case FSIZE8_6   :	page = 1 ; line = 6  ;break; 
			#endif
			#if FONT12
				case FSIZE12_6  :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
			#if FONT16
				case FSIZE16_8  :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
			#if FONT24
				case FSIZE24_12 :	page = (Size-1)/8+1 ; line = Size/2 ;break;
			#endif
		}
		while(*(Data+strcount) != '\0')
		{
			OLED_ShowChar(x + line*strcount , y , *(Data+strcount), Size , 0);
			strcount++;
		}
		if(RefreshDisplay == 1)
		{
			OLED_RefreshAreaRAM(x, y, x+line*Strdatalenght, y+8*page);
		}
	}
}

/**
 * ************************************************************************
 * @brief 中文显示函数
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     Data  	汉字
 * @param     Size  	汉字大小 12,16,24
 * @param     RefreshDisplay  		设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char *Data,unsigned char Size , unsigned int RefreshDisplay)
{      	
	if (x < SCREEN_COLUMN && y < SCREEN_ROW ) 
	{
		int32_t position = 0;
		unsigned char page,line;
		unsigned char temp;
		for(position = 0 ; position < HZ_LENGHT ; )  //查汉字位置
		{
			if( *Data == FONT_HZ[position] && *(Data + 1) == FONT_HZ[position+1] )
				break; 
			position +=2;
			if(position >= HZ_LENGHT)
				return;
		}
		position = position/2;
		switch(Size)					//获取字符占用页，占用列
		{
			#if FONT8 
				case FSIZE8_6   :	return; 
			#endif
			#if FONT12
				case FSIZE12_6  :	page = (Size-1)/8+1 ; line = Size ;break;
			#endif
			#if FONT16
				case FSIZE16_8  :	page = (Size-1)/8+1 ; line = Size ;break;
			#endif
			#if FONT24
				case FSIZE24_12 :	page = (Size-1)/8+1 ; line = Size ;break;
			#endif
		}
		for(unsigned char m = 0; m < page; m++)
		{
			for(unsigned char n = 0; n < line; n++)
			{
				switch(Size)
				{
					#if FONT12
						case FSIZE12_6  :	temp = F12X12_CN[position][n+m*line] ;break;
					#endif
					#if FONT16
						case FSIZE16_8  :	temp = F16X16_CN[position][n+m*line] ;break;
					#endif
					#if FONT24
						case FSIZE24_12 :	temp = F24X24_CN[position][n+m*line] ;break;
					#endif
				}
				for(unsigned char i = 0; i < 8; i++)
				{
					OLED_DrawPoint(x+n, y+i+m*8, (temp >> i) & 0x01 , 0);
				}
			}	
		}
		
		if(RefreshDisplay == 1)
		{
			OLED_RefreshAreaRAM(x, y, x+line, y+8*page);
		}
	}
}

/**
 * ************************************************************************
 * @brief 字符串或者中文显示函数
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     StringChinese  	字符串或者中文
 * @param     Size  			大小 12,16,24
 * @param     RefreshDisplay  	设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_ShowStringChinese(unsigned char x,unsigned char y,unsigned char *Data,unsigned char Size , unsigned int RefreshDisplay)
{
	unsigned int datalenght= strlen((char *)Data);
	unsigned int datacount = 0;
	unsigned char page,line;
	switch(Size)					//获取字符占用页，占用列
	{
		#if FONT8 
			case FSIZE8_6   :	return; 
		#endif
		#if FONT12
			case FSIZE12_6  :	page = (Size-1)/8+1 ; line = Size/2  ;break;
		#endif
		#if FONT16
			case FSIZE16_8  :	page = (Size-1)/8+1 ; line = Size/2  ;break;
		#endif
		#if FONT24
			case FSIZE24_12 :	page = (Size-1)/8+1 ; line = Size/2 ;break;
		#endif
	}
	while(*(Data+datacount) !=0 )//数据未结束
	{ 
		if(*(Data+datacount)  > 0x80)//中文 
		{	

			OLED_ShowChinese(x +line*datacount , y , (Data+datacount), Size , 0);
			datacount += 2; 
		}
		else              			//字符
		{          
			OLED_ShowChar(x +line*datacount , y , *(Data+datacount), Size , 0);
			datacount++;
		}						 
	} 
	if(RefreshDisplay == 1)
	{
		OLED_RefreshAreaRAM(x, y, x+line*datalenght, y+8*page);
	}
}

/**
 * ************************************************************************
 * @brief 字符串或者中文显示函数,支持使用vsnprintf转换的方方法
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     StringChinese  	字符串或者中文
 * @param     Size  			大小 12,16,24
 * @param     RefreshDisplay  	设置刷新显示 RefreshDisplay = 1,设置不刷新显?? RefreshDisplay = 0
 * @param     ...  	表示可变参数
 * 
 * @note     字符串最大支持30-1个字符
 * ************************************************************************
 */
void OLED_PrintfStringChinese(unsigned char x,unsigned char y, unsigned char *Data,unsigned char Size , unsigned int RefreshDisplay , ...)
{
	va_list ap;
	unsigned char printfbuffer[30];
	va_start(ap , RefreshDisplay);												//开始处理可变参数
	vsnprintf((char *)printfbuffer, sizeof(printfbuffer) ,(char *)Data, ap);	//使用vsnprintf做数据转换
	va_end(ap);																	//结束处理可变参数
	OLED_ShowStringChinese(x ,y , printfbuffer , Size ,RefreshDisplay);			//调用字符串或者中文显示函数
}


/**
 * ************************************************************************
 * @brief BMP图片显示函数
 * 
 * @param     x  	起始点横坐标（0~127）
 * @param     y  	起始点纵坐标（0~63）
 * @param     L  	BMP图片宽度
 * @param     H  	BMP图片高度
 * @param     BMP  	图片取模地址
 * @param     RefreshDisplay  	设置刷新显示 RefreshDisplay = 1,设置不刷新显示 RefreshDisplay = 0
 * 
 * ************************************************************************
 */
void OLED_ShowBMP(unsigned char x,unsigned char y,unsigned char MapWidth,unsigned char MapHeight,const unsigned char *BMP, unsigned int RefreshDisplay)
{
	if (x < SCREEN_COLUMN && x+MapWidth <= SCREEN_COLUMN && y < SCREEN_ROW && y+MapHeight <= SCREEN_ROW) 
	{
		unsigned char *p = (unsigned char *)BMP;
		for(unsigned char height = y; height < y+MapHeight; height += 8)
		{
			for(unsigned char width = x; width < x+MapWidth; width ++)
			{
				for(unsigned char i = 0; i < 8; i ++)
				{
					OLED_DrawPoint(width, height+i, ((*p) >> i) & 0x01 , 0);
				}
				p++;
			}
		}
		if(RefreshDisplay == 1)
		{
			OLED_RefreshAreaRAM(x, y, x+MapWidth, y+MapHeight);
		}
	}
	
}


/**
 * ************************************************************************
 * @brief OLED初始化函数
 * 
 * ************************************************************************
 */
void OLED_Init(void)
{
	OLED_WriteCmd(0xAE); 	//显示关闭
	OLED_WriteCmd(0x20);	//设置内存寻址模式
	OLED_WriteCmd(0x10);	//00,水平寻址模式;01,垂直寻址模式;10,页寻址模式(复位);11,无效
	OLED_WriteCmd(0xb0);	//设置页寻址模式的页起始地址,0-7
	OLED_WriteCmd(0xc8);	//设置COM输出扫描方向
	OLED_WriteCmd(0x00); 	//-设置低列地址
	OLED_WriteCmd(0x10); 	//-设置高列地址
	OLED_WriteCmd(0x40); 	//-设置起始行地址
	OLED_WriteCmd(0x81); 	//设置对比度控制寄存器
	OLED_WriteCmd(0xff); 	//亮度调节 0x00~0xff
	OLED_WriteCmd(0xa1); 	//设置段重新映射0到127
	OLED_WriteCmd(0xa6); 	//设置正常显示
	OLED_WriteCmd(0xa8); 	//设置复用比例(1到64)
	OLED_WriteCmd(0x3F); 	//
	OLED_WriteCmd(0xa4); 	//0xa4,输出遵循RAM内容;0xa5,输出忽略RAM内容
	OLED_WriteCmd(0xd3); 	//设置显示偏移
	OLED_WriteCmd(0x00); 	//不偏移
	OLED_WriteCmd(0xd5); 	//--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0xf0); 	//--set divide ratio
	OLED_WriteCmd(0xd9); 	//--set pre-charge period
	OLED_WriteCmd(0x22); 	//
	OLED_WriteCmd(0xda); 	//--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); 	//--set vcomh
	OLED_WriteCmd(0x20); 	//0x20,0.77xVcc
	OLED_WriteCmd(0x8d); 	//设置DC-DC使能
	OLED_WriteCmd(0x14); 	//
	OLED_WriteCmd(0xaf); 	//--turn on oled panel
	OLED_Clear(); 			//清屏
}

