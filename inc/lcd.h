/*
 *  @file  : lcd.h
 *	
 *  @brief	based on LonelyWolf repository https://github.com/LonelyWolf/stm32/tree/master/ST7735
 *
 *  @author: franc
 *  @date  : 25 nov 2019
 */

#ifndef LCD_H_
#define LCD_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "spi.h"

/* Screen resolution */
#define LCD_SCREEN_W         128
#define LCD_SCREEN_H         128

// ST7735 A0 (Data/Command select) pin
#define ST7735_A0_PORT     GPIOB
#define ST7735_A0_PIN      GPIO_Pin_4    // PB4

// ST7735 RST (Reset) pin
#define ST7735_RST_PORT    GPIOB
#define ST7735_RST_PIN     GPIO_Pin_6    // PB6

// ST7735 CS (Chip Select) pin
#define ST7735_CS_PORT     GPIOB
#define ST7735_CS_PIN      GPIO_Pin_7    // PB7

// CS pin macros
#define CS_L() GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define CS_H() GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

// A0 pin macros
#define A0_L() GPIO_ResetBits(LCD_A0_GPIO_PORT, LCD_A0_PIN)
#define A0_H() GPIO_SetBits(LCD_A0_GPIO_PORT, LCD_A0_PIN)

// RESET pin macros
#define RST_L() GPIO_ResetBits(LCD_RESET_GPIO_PORT, LCD_RESET_PIN)
#define RST_H() GPIO_SetBits(LCD_RESET_GPIO_PORT, LCD_RESET_PIN)

typedef enum {
	eLCD_orientation_normal 		= 0,
	eLCD_orientation_CW     		= 1,
	eLCD_orientation_normal_CCW    	= 2,
	eLCD_orientation_normal_180    	= 3,
	eLCD_orientation_max
} tLCD_orientation;

extern void ST7735_write(uint8_t data);

extern uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B);

extern void ST7735_Init(void);
extern void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
extern void ST7735_Orientation(uint8_t orientation);
extern void ST7735_Clear(uint16_t color);

extern void ST7735_Pixel(uint16_t X, uint16_t Y, uint16_t color);
extern void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color);
extern void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color);
extern void ST7735_Line(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color);
extern void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
extern void ST7735_FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);

extern void ST7735_PutChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
extern void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color);

extern void Lcd_write_reg(uint8_t reg, uint8_t data);
extern void Lcd_reset(void);

#endif /* LCD_H_ */
