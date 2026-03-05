#ifndef __IIC_OLED_FONT_H
#define __IIC_OLED_FONT_H 	   

//字体设置为1的可以使用。
#define	FONT8	1
#define	FONT12	1
#define	FONT16	1
#define	FONT24	1

#if FONT8
#define FSIZE8_6	8
extern const unsigned char F8X6[][6];
#endif

#if FONT12
#define FSIZE12_6	12	//小五号字体
extern const unsigned char F12X6[][12];
extern const unsigned char F12X12_CN[][24];
#endif

#if FONT16
#define FSIZE16_8	16	//小四号字体
extern const unsigned char F16X8[][16];
extern const unsigned char F16X16_CN[][32];
#endif

#if FONT24
#define FSIZE24_12	24	//小二号字体
extern const unsigned char F24X12[][36];
extern const unsigned char F24X24_CN[][72];
#endif

//汉字字符表
#define HZ_LENGHT  255
extern const char  FONT_HZ[];

#endif


