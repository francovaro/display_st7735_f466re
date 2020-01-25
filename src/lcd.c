/*
 *  @file  : lcd.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 25 nov 2019
 */
#include <string.h>

#include "lcd.h"
#include "delay.h"
#include "font5x7.h"
#include "st7735_cmd.h"

/**
 *
 * @param data
 */
void ST7735_write(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, data);
}

/**
 *
 * @param reg
 * @param data
 */
void Lcd_write_reg(uint8_t reg, uint8_t data)
{
	ST7735_cmd(reg);
	ST7735_data(data);
}

/**
 *
 * @param cmd
 */
void ST7735_cmd(uint8_t cmd)
{
	A0_L();
	ST7735_write(cmd);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);
}

void ST7735_data(uint8_t data)
{
	A0_H();
	ST7735_write(data);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);
}

/**
 *
 * @param R red
 * @param G green
 * @param B blue
 * @return the uin16 rapresentation of the signle R G B color
 */
uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B)
{
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE)
{
	CS_L();
	ST7735_cmd(0x2a); // Column address set
	A0_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b); // Row address set
	A0_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c); // Memory write
	CS_H();
}

void ST7735_Clear(uint16_t color)
{
	uint16_t i;
	uint8_t  CH;
	uint8_t  CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(0,0,LCD_SCREEN_W - 1,LCD_SCREEN_H - 1);
	A0_H();
	for (i = 0; i < LCD_SCREEN_W * LCD_SCREEN_H; i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
{
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X1,Y,X2,Y);
	A0_H();
	for (i = 0; i <= (X2 - X1); i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
{
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X,Y1,X,Y2);
	A0_H();
	for (i = 0; i <= (Y2 - Y1); i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Line(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color)
{
	int16_t dX = X2-X1;
	int16_t dY = Y2-Y1;
	int16_t dXsym = (dX > 0) ? 1 : -1;
	int16_t dYsym = (dY > 0) ? 1 : -1;

	if (dX == 0)
	{
		if (Y2>Y1) 
		{
			ST7735_VLine(X1,Y1,Y2,color); 
		}
		else
		{
			ST7735_VLine(X1,Y2,Y1,color);
		}		 
		return;
	}
	if (dY == 0)
	{
		if (X2>X1) 
		{
			ST7735_HLine(X1,X2,Y1,color); 
		}
		else
		{
			ST7735_HLine(X2,X1,Y1,color);
		} 
		return;
	}

	dX *= dXsym;
	dY *= dYsym;
	int16_t dX2 = dX << 1;
	int16_t dY2 = dY << 1;
	int16_t di;

	if (dX >= dY)
	{
		di = dY2 - dX;
		while (X1 != X2)
		{
			ST7735_Pixel(X1,Y1,color);
			X1 += dXsym;
			if (di < 0)
			{
				di += dY2;
			}
			else
			{
				di += dY2 - dX2;
				Y1 += dYsym;
			}
		}
	}
	else
	{
		di = dX2 - dY;
		while (Y1 != Y2)
		{
			ST7735_Pixel(X1,Y1,color);
			Y1 += dYsym;
			if (di < 0)
			{
				di += dX2;
			}
			else
			{
				di += dX2 - dY2;
				X1 += dXsym;
			}
		}
	}
	ST7735_Pixel(X1,Y1,color);
}

/**
 *@brief
 */
void Lcd_reset(void)
{
	RST_H();
	Delay_ms(1);
	RST_L();
	Delay_ms(1);
	RST_H();
	Delay_ms(150);
}

void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	CS_L();
	ST7735_HLine(X1,X2,Y1,color);
	ST7735_HLine(X1,X2,Y2,color);
	ST7735_VLine(X1,Y1,Y2,color);
	ST7735_VLine(X2,Y1,Y2,color);
	CS_H();
}

void ST7735_FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	uint16_t i;
	uint16_t FS = (X2 - X1 + 1) * (Y2 - Y1 + 1);
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(X1,Y1,X2,Y2);
	A0_H();
	for (i = 0; i < FS; i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_PutChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color)
{
	uint16_t i,j;
	uint8_t buffer[5];
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	memcpy(buffer,&Font5x7[(chr - 32) * 5],5);

	CS_L();
	ST7735_AddrSet(X,Y,X + 4,Y + 6);
	A0_H();
	for (j = 0; j < 7; j++)
	{
        for (i = 0; i < 5; i++)
        {
    		if ((buffer[i] >> j) & 0x01)
    		{
    			ST7735_write(CH);
    			ST7735_write(CL);
    		}
    		else
    		{
    			ST7735_write(0x00);
    			ST7735_write(0x00);
    		}
    	}
    }
	CS_H();
}
#if 0
void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color)
{
    while (*str)
    {
        ST7735_PutChar5x7(X,Y,*str++,color);
        if (X < _lcd_screen_w - 6)
        {
        	X += 6;
        }
        else if (Y < _lcd_screen_h - 8)
        {
        	X = 0; Y += 8;
        }
        else
        {
        	X = 0; Y = 0;
        }
    };
}
#endif

#if 0
void Lcd_lonely_init(void)
{
	Lcd_reset();	// Reset display
	/*
	CS_H();
	RST_H();
	Delay_ms(5);
	RST_L();
	Delay_ms(5);
	RST_H();
	CS_H();
	Delay_ms(5);*/
	CS_L();

	ST7735_cmd(0x11); // Sleep out & booster on
	Delay_ms(130); // Datasheet says what display wakes about 120ms (may be much faster actually)

	ST7735_cmd(0xb1);   // In normal mode (full colors):
	A0_H();
	ST7735_write(0x05); //   RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c); //   Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c); //   Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(0xb2);   // In idle mode (8-colors):
	A0_H();
	ST7735_write(0x05); //   RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c); //   Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c); //   Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(0xb3);   // In partial mode + full colors:
	A0_H();
	ST7735_write(0x05); //   RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c); //   Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c); //   Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05); //   RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c); //   Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c); //   Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(0xB4);   // Display dot inversion control:
	ST7735_data(0x03);  //   NLB,NLC

	ST7735_cmd(0x3a);   // Interface pixel format
//	ST7735_data(0x03);  // 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);  // 16-bit/pixel RGB 5-6-5 (65k colors)
//	ST7735_data(0x06);  // 18-bit/pixel RGB 6-6-6 (256k colors)

//	ST7735_cmd(0x36);   // Memory data access control:
						//   MY MX MV ML RGB MH - -
//	ST7735_data(0x00);  //   Normal: Top to Bottom; Left to Right; RGB
//	ST7735_data(0x80);  //   Y-Mirror: Bottom to top; Left to Right; RGB
//	ST7735_data(0x40);  //   X-Mirror: Top to Bottom; Right to Left; RGB
//	ST7735_data(0xc0);  //   X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
//	ST7735_data(0x20);  //   X-Y Exchange: X and Y changed positions
//	ST7735_data(0xA0);  //   X-Y Exchange,Y-Mirror
//	ST7735_data(0x60);  //   X-Y Exchange,X-Mirror
//	ST7735_data(0xE0);  //   X-Y Exchange,X-Mirror,Y-Mirror

	ST7735_cmd(0x20);   // Display inversion off
//	ST7735_cmd(0x21);   // Display inversion on

	ST7735_cmd(0x13);   // Partial mode off

	ST7735_cmd(0x26);   // Gamma curve set:
	ST7735_data(0x01);  //   Gamma curve 1 (G2.2) or (G1.0)
//	ST7735_data(0x02);  //   Gamma curve 2 (G1.8) or (G2.5)
//	ST7735_data(0x04);  //   Gamma curve 3 (G2.5) or (G2.2)
//	ST7735_data(0x08);  //   Gamma curve 4 (G1.0) or (G1.8)

	ST7735_cmd(0x29);   // Display on

	CS_H();

	//ST7735_Orientation(eLCD_orientation_normal);
}

void Lcd_china_init(void)
{
 	Lcd_reset();

	CS_L();

	ST7735_cmd(0x11);//Sleep exit
	Delay_ms (120);
	//ST7735R Frame Rate
	ST7735_cmd(0xB1);
	ST7735_data(0x01);
	ST7735_data(0x2C);
	ST7735_data(0x2D);

	ST7735_cmd(0xB2);
	ST7735_data(0x01);
	ST7735_data(0x2C);
	ST7735_data(0x2D);

	ST7735_cmd(0xB3);
	ST7735_data(0x01);
	ST7735_data(0x2C);
	ST7735_data(0x2D);
	ST7735_data(0x01);
	ST7735_data(0x2C);
	ST7735_data(0x2D);

	ST7735_cmd(0xB4); //Column inversion
	ST7735_data(0x07);
	//ST7735R Power Sequence
	ST7735_cmd(0xC0);
	ST7735_data(0xA2);
	ST7735_data(0x02);
	ST7735_data(0x84);

	ST7735_cmd(0xC1);
	ST7735_data(0xC5);

	ST7735_cmd(0xC2);
	ST7735_data(0x0A);
	ST7735_data(0x00);

	ST7735_cmd(0xC3);
	ST7735_data(0x8A);
	ST7735_data(0x2A);

	ST7735_cmd(0xC4);
	ST7735_data(0x8A);
	ST7735_data(0xEE);

	ST7735_cmd(0xC5); //VCOM
	ST7735_data(0x0E);

	ST7735_cmd(0x36); //MX, MY, RGB mode
	ST7735_data(0xC8);
	//ST7735R Gamma Sequence
	ST7735_cmd(0xe0);
	ST7735_data(0x0f);
	ST7735_data(0x1a);
	ST7735_data(0x0f);
	ST7735_data(0x18);
	ST7735_data(0x2f);
	ST7735_data(0x28);
	ST7735_data(0x20);
	ST7735_data(0x22);
	ST7735_data(0x1f);
	ST7735_data(0x1b);
	ST7735_data(0x23);
	ST7735_data(0x37);
	ST7735_data(0x00);
	ST7735_data(0x07);
	ST7735_data(0x02);
	ST7735_data(0x10);

	ST7735_cmd(0xe1);
	ST7735_data(0x0f);
	ST7735_data(0x1b);
	ST7735_data(0x0f);
	ST7735_data(0x17);
	ST7735_data(0x33);
	ST7735_data(0x2c);
	ST7735_data(0x29);
	ST7735_data(0x2e);
	ST7735_data(0x30);
	ST7735_data(0x30);
	ST7735_data(0x39);
	ST7735_data(0x3f);
	ST7735_data(0x00);
	ST7735_data(0x07);
	ST7735_data(0x03);
	ST7735_data(0x10);

	ST7735_cmd(0x2a);
	ST7735_data(0x00);
	ST7735_data(0x00);
	ST7735_data(0x00);
	ST7735_data(0x7f);

	ST7735_cmd(0x2b);
	ST7735_data(0x00);
	ST7735_data(0x00);
	ST7735_data(0x00);
	ST7735_data(0x9f);

	ST7735_cmd(0xF0); //Enable test command
	ST7735_data(0x01);

	ST7735_cmd(0xF6); //Disable ram power save mode
	ST7735_data(0x00);

	ST7735_cmd(0x3A); //65k mode
	ST7735_data(0x05);

	ST7735_cmd(0x29);//Display on

	CS_H();

	ST7735_Orientation(eLCD_orientation_normal);
}
#endif
