/*
 *  @file  : lcd.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 25 nov 2019
 */

#include "lcd.h"
#include "delay.h"

static uint8_t _lcd_screen_w;
static uint8_t _lcd_screen_h;

static void ST7735_cmd(uint8_t cmd);
static void ST7735_data(uint8_t data);

void ST7735_write(uint8_t data)
{
#ifdef SOFT_SPI
	uint8_t i;

	for(i = 0; i < 8; i++) {
		if (data & 0x80) SDA_H(); else SDA_L();
		data = data << 1;
		SCK_L();
		SCK_H();
	}
#else
	CS_L();
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, data);
	CS_H();
#endif
}

void Lcd_write_reg(uint8_t reg, uint8_t data)
{
	ST7735_cmd(reg);
	ST7735_data(data);
}

static void ST7735_cmd(uint8_t cmd)
{
	A0_L();
	ST7735_write(cmd);
#ifndef SOFT_SPI
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);
#endif
}

static void ST7735_data(uint8_t data)
{
	A0_H();
	ST7735_write(data);
#ifndef SOFT_SPI
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);
#endif
}

uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B)
{
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

void ST7735_Orientation(tLCD_orientation orientation)
{

	ST7735_cmd(0x36); // Memory data access control:
	switch(orientation)
	{
		case eLCD_orientation_CW:
		{
			_lcd_screen_w  = LCD_SCREEN_H;
			_lcd_screen_h = LCD_SCREEN_W;
			ST7735_data(0xA0); // X-Y Exchange,Y-Mirror
		}
		break;
		case eLCD_orientation_normal_CCW:
		{
			_lcd_screen_w  = LCD_SCREEN_H;
			_lcd_screen_h = LCD_SCREEN_W;
			ST7735_data(0x60); // X-Y Exchange,X-Mirror
		}
		break;
		case eLCD_orientation_normal_180:
		{
			_lcd_screen_w  = LCD_SCREEN_W;
			_lcd_screen_h = LCD_SCREEN_H;
			ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		}
		break;
		default:
		{
			_lcd_screen_w  = LCD_SCREEN_W;
			_lcd_screen_h = LCD_SCREEN_H;
			ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		}
		break;
	}
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE)
{
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
}

void ST7735_Clear(uint16_t color)
{
	uint16_t i;
	uint8_t  CH;
	uint8_t  CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(0,0,_lcd_screen_w - 1,_lcd_screen_h - 1);
	A0_H();
	for (i = 0; i < _lcd_screen_w * _lcd_screen_h; i++)
	{
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Pixel(uint16_t X, uint16_t Y, uint16_t color)
{
    CS_L();
    ST7735_AddrSet(X,Y,X,Y);
    A0_H();
    ST7735_write(color >> 8);
    ST7735_write((uint8_t)color);
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
		if (Y2>Y1) ST7735_VLine(X1,Y1,Y2,color); else ST7735_VLine(X1,Y2,Y1,color);
		return;
	}
	if (dY == 0)
	{
		if (X2>X1) ST7735_HLine(X1,X2,Y1,color); else ST7735_HLine(X2,X1,Y1,color);
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

void Lcd_reset(void)
{
	RST_L();
	Delay_ms(100);
	RST_H();
	Delay_ms(50);
}

void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	ST7735_HLine(X1,X2,Y1,color);
	ST7735_HLine(X1,X2,Y2,color);
	ST7735_VLine(X1,Y1,Y2,color);
	ST7735_VLine(X2,Y1,Y2,color);
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

void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color)
{
    while (*str)
    {
        ST7735_PutChar5x7(X,Y,*str++,color);
        if (X < _lcd_screen_w - 6) { X += 6; } else if (Y < _lcd_screen_h - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
    };
}
