#ifndef  __IIC_OLED_H
#define  __IIC_OLED_H

#include "gpio.h"
#include "delay.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "iic_oled_font.h" 
#include "iic_oled_bmp.h" 

//与硬件引脚相关的定义，根据配置引脚名称会自动关联
#define	OLED_SCLK_SET_1		HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port , OLED_SCLK_Pin , GPIO_PIN_SET)
#define	OLED_SCLK_SET_0		HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port , OLED_SCLK_Pin , GPIO_PIN_RESET)
#define	OLED_SDA_SET_1		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port  , OLED_SDA_Pin  , GPIO_PIN_SET)
#define	OLED_SDA_SET_0		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port  , OLED_SDA_Pin  , GPIO_PIN_RESET)
#define	OLED_SDA_GET		HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port   , OLED_SDA_Pin)

//延时函数从定义名称
#define		OLED_DelayMs		DelayMs
#define		OLED_DelayUs		DelayUs

//OLED液晶相关的参数
#define    	SCREEN_PAGE_NUM		(8)     //屏幕页数
#define    	SCREEN_PAGEDATA_NUM	(128)   //每页的数据个数
#define		SCREEN_COLUMN		(128)   //列数
#define  	SCREEN_ROW			(64)    //行数
#define 	OLED_ADDRESS 		0x78	//OLED地址  默认0x78

//函数声明
void OLED_SDA_Out(void);

void OLED_SDA_In(void);

void OLED_IIC_Start(void);

void OLED_IIC_Stop(void);

unsigned char OLED_IIC_WaitAck(void);

void OLED_IIC_WriteByte(unsigned char Data);

void OLED_WriteCmd(unsigned char Cmd);

void OLED_WriteData(unsigned char Data);

void OLED_DisplayOn(void);

void OLED_DisplayOff(void);

void OLED_Clear(void);

void OLED_RefreshRAM(void);

void OLED_RefreshAreaRAM(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void OLED_ClearRAM(void);

void OLED_DisplayMode(unsigned char Mode);

void OLED_IntensityControl(unsigned char Intensity);

void OLED_Shift(unsigned char ShiftNum);

void OLED_HorizontalShift(unsigned char StartPage, unsigned char EndPage, unsigned char Direction);

void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char DrawPoint, unsigned int RefreshDisplay);

void OLED_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char DrawLine, unsigned int RefreshDisplay);

void OLED_DrawRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char DrawRectangle, unsigned int RefreshDisplay);

void LCD_DrawCircle(unsigned char x, unsigned char y, unsigned char Radius, unsigned char DrawCircle, unsigned int RefreshDisplay);

void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char Data, unsigned char Size, unsigned int RefreshDisplay);

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *Data, unsigned char Size, unsigned int RefreshDisplay);

void OLED_ShowChinese(unsigned char x, unsigned char y, unsigned char *Data, unsigned char Size, unsigned int RefreshDisplay);

void OLED_ShowStringChinese(unsigned char x, unsigned char y, unsigned char *Data, unsigned char Size, unsigned int RefreshDisplay);

void OLED_PrintfStringChinese(unsigned char x, unsigned char y, unsigned char *Data, unsigned char Size, unsigned int RefreshDisplay, ...);

void OLED_ShowBMP(unsigned char x, unsigned char y, unsigned char MapWidth, unsigned char MapHeight, const unsigned char *BMP, unsigned int RefreshDisplay);

void OLED_Init(void);

#endif


