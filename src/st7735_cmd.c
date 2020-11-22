/*
 *  @file  : st7735_cmd.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 29 nov 2019
 */

#include <stdint.h>
#include "st7735_cmd.h"
#include "lcd.h"
#include "spi.h"
#include "delay.h"

uint8_t _lcd_screen_w;
uint8_t _lcd_screen_h;

#define NULL 0;

static tLCD_cmd _lcd_sys_cmd[e_syscmd_max];
static tLCD_cmd _lcd_panel_cmd[e_panelcmd_max];

/*{ 0x00, 0x06, 0x3D} */
/*{ 0x01, 0x2C, 0x2D} */
/*{ 0x02, 0x35, 0x36} */
static uint8_t FRMCTR1_buffer[3] = { 0x02, 0x35, 0x36};
static uint8_t FRMCTR2_buffer[3] = { 0x02, 0x35, 0x36};
static uint8_t FRMCTR3_buffer[6] = { 0x02, 0x35, 0x36, 0x02, 0x35, 0x36};
//static uint8_t FRMCTR1_buffer[3] = { 0x01, 0x2C, 0x2D};
//static uint8_t FRMCTR2_buffer[3] = { 0x01, 0x2C, 0x2D};
//static uint8_t FRMCTR3_buffer[6] = { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D};

static uint8_t PWCTR1_buffer[3] = { 0xA2, 0x02, 0x84};
static uint8_t PWCTR2_buffer[1] = { 0xC5};
/* { 0x0A, 0x00}; */
static uint8_t PWCTR3_buffer[2] = { 0x0A, 0x00};
/* */
static uint8_t PWCTR4_buffer[2] = { 0x8A, 0x2A};
/* */
static uint8_t PWCTR5_buffer[2] = { 0x8A, 0xEE};

static uint8_t VMCTR1_buffer[1] = { 0x0E};	/* {0x3C,0x38}; ?*/

static uint8_t INVCTR_buffer[1] = { 0x07};	/* 0x00 or 0x07 or 0x03*/

/*
 * GAMSET
 * 01
 * 02
 * 04
 * 08
 */
static uint8_t GAMSET_buffer_buffer[1] = { 0x01};

static uint8_t GAMCTRP1_buffer[16] = {0x12, 0x1C, 0x10, 0x18, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 };
static uint8_t GAMCTRN1_buffer[16] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 };

//static uint8_t GAMCTRP1_buffer[16] = {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 };
//static uint8_t GAMCTRN1_buffer[16] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 };

//static uint8_t GAMCTRP1_buffer[16] = {0x0f, 0x1a, 0x0f, 0x18, 0x2f, 0x28, 0x20, 0x22, 0x1f, 0x1b, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10 };
//static uint8_t GAMCTRN1_buffer[16] = {0x0f, 0x1b, 0x0f, 0x17, 0x33, 0x2C, 0x29, 0x2e, 0x30, 0x30, 0x39, 0x3F, 0x00, 0x07, 0x03, 0x10 };

static uint8_t CASET_buffer_buffer[4] 	= {0x00, 0x00, 0x00, 0x7F};	/* line number 0...127*/
static uint8_t RASET_buffer_buffer[4] 	= {0x00, 0x00, 0x00, 0x7F}; /* line number 0...127*/

/* COLMOD
 * 0x03 12 bit/pixel
 * 0x05 16 bit/pixel
 * 0x06 18 bit/pixel
 * 0x07 No used
 * */
static uint8_t COLMOD_buffer_buffer[1] 	= {0x05};

static uint8_t MAD_CTL_buffer[1] = {0x00};	/* was 0 */


void ST7735_sys_cmd_init(void)
{    
    /* Software Reset */
    _lcd_sys_cmd[SYS_CMD_SWRESET].cmd = 0x01;
    _lcd_sys_cmd[SYS_CMD_SWRESET].nrOfByte = 0;
    _lcd_sys_cmd[SYS_CMD_SWRESET].data = NULL;

    /* Sleep ON */
	_lcd_sys_cmd[SLPIN].cmd = 0x10;
	_lcd_sys_cmd[SLPOUT].nrOfByte = 0;
	_lcd_sys_cmd[SLPIN].data = NULL;

    /* Sleep Out & Booster On */
    _lcd_sys_cmd[SLPOUT].cmd = 0x11;
    _lcd_sys_cmd[SLPOUT].nrOfByte = 0;
    _lcd_sys_cmd[SLPOUT].data = NULL;

    /* Memory Data Access Control */
    _lcd_sys_cmd[MADCTL].cmd = 0x36;
    _lcd_sys_cmd[MADCTL].nrOfByte = 1;
    _lcd_sys_cmd[MADCTL].data = MAD_CTL_buffer;
    
    /* Column Address Set */
    _lcd_sys_cmd[CASET].cmd = 0x2A;
    _lcd_sys_cmd[CASET].nrOfByte = 4;
    _lcd_sys_cmd[CASET].data = CASET_buffer_buffer;

    /* Row Address Set */
    _lcd_sys_cmd[RASET].cmd = 0x2B;
    _lcd_sys_cmd[RASET].nrOfByte = 4;
    _lcd_sys_cmd[RASET].data = RASET_buffer_buffer;

    /* Display ON */
    _lcd_sys_cmd[DISPON].cmd = 0x29;
	_lcd_sys_cmd[DISPON].nrOfByte = 0;
	_lcd_sys_cmd[DISPON].data = NULL;

	/* Display OFF */
	_lcd_sys_cmd[DISPOFF].cmd = 0x28;
	_lcd_sys_cmd[DISPOFF].nrOfByte = 0;
	_lcd_sys_cmd[DISPOFF].data = NULL;

    /* Interface Pixel Format */
    _lcd_sys_cmd[COLMOD].cmd = 0x3A;
    _lcd_sys_cmd[COLMOD].nrOfByte = 1;
    _lcd_sys_cmd[COLMOD].data = COLMOD_buffer_buffer;

    /* Display Inversion Off (Normal) */
    _lcd_sys_cmd[INVOFF].cmd = 0x20;
    _lcd_sys_cmd[INVOFF].nrOfByte = 0;
    _lcd_sys_cmd[INVOFF].data = NULL;


    /* VCOM Control 1 */
	_lcd_sys_cmd[NORON].cmd = 0x13;
	_lcd_sys_cmd[NORON].nrOfByte = 0;
	_lcd_sys_cmd[NORON].data = NULL;

    /* VCOM Control 1 */
	_lcd_sys_cmd[GAMSET].cmd = 0x26;
	_lcd_sys_cmd[GAMSET].nrOfByte = 1;
	_lcd_sys_cmd[GAMSET].data = GAMSET_buffer_buffer;

	/* Memory Write */
	_lcd_sys_cmd[RAMWR].cmd = 0x2C;
	_lcd_sys_cmd[RAMWR].nrOfByte = 0;
	_lcd_sys_cmd[RAMWR].data = NULL;

	/* IDLE MODE OFF */
	_lcd_sys_cmd[IDMOFF].cmd = 0x38;
	_lcd_sys_cmd[IDMOFF].nrOfByte = 0;
	_lcd_sys_cmd[IDMOFF].data = NULL;

	/* IDLE MODE ON */
	_lcd_sys_cmd[IDMON].cmd = 0x39;
	_lcd_sys_cmd[IDMON].nrOfByte = 0;
	_lcd_sys_cmd[IDMON].data = NULL;


}


void ST7735_panel_cmd_init(void)
{
    /* In Normal Mode (Full Colors) */
    _lcd_panel_cmd[FRMCTR1].cmd = 0xB1;
    _lcd_panel_cmd[FRMCTR1].nrOfByte = 3;
    _lcd_panel_cmd[FRMCTR1].data = FRMCTR1_buffer;

    /* In Idle Mode (8-colors) */
    _lcd_panel_cmd[FRMCTR2].cmd = 0xB2;
    _lcd_panel_cmd[FRMCTR2].nrOfByte = 3;
    _lcd_panel_cmd[FRMCTR2].data = FRMCTR2_buffer;

    /* In Partial Mode + FullColors */
    _lcd_panel_cmd[FRMCTR3].cmd = 0xB3;
    _lcd_panel_cmd[FRMCTR3].nrOfByte = 6;
    _lcd_panel_cmd[FRMCTR3].data = FRMCTR3_buffer;

    /*  Power Control Setting */
    _lcd_panel_cmd[PWCTR1].cmd = 0xC0;
    _lcd_panel_cmd[PWCTR1].nrOfByte = 3;
    _lcd_panel_cmd[PWCTR1].data = PWCTR1_buffer;

    /* Power Control Setting */
    _lcd_panel_cmd[PWCTR2].cmd = 0xC1;
    _lcd_panel_cmd[PWCTR2].nrOfByte = 1;
    _lcd_panel_cmd[PWCTR2].data = PWCTR2_buffer;

    _lcd_panel_cmd[PWCTR3].cmd = 0xC2;
    _lcd_panel_cmd[PWCTR3].nrOfByte = 2;
    _lcd_panel_cmd[PWCTR3].data = PWCTR3_buffer;

    _lcd_panel_cmd[PWCTR4].cmd = 0xC3;
    _lcd_panel_cmd[PWCTR4].nrOfByte = 2;
    _lcd_panel_cmd[PWCTR4].data = PWCTR4_buffer;

    _lcd_panel_cmd[PWCTR5].cmd = 0xC4;
    _lcd_panel_cmd[PWCTR5].nrOfByte = 2;
    _lcd_panel_cmd[PWCTR5].data = PWCTR5_buffer;

    /* VCOM Control 1 */
    _lcd_panel_cmd[VMCTR1].cmd = 0xC5;
    _lcd_panel_cmd[VMCTR1].nrOfByte = 1;
    _lcd_panel_cmd[VMCTR1].data = VMCTR1_buffer;

    _lcd_panel_cmd[GAMCTRP1].cmd = 0xE0;
    _lcd_panel_cmd[GAMCTRP1].nrOfByte = 16;
    _lcd_panel_cmd[GAMCTRP1].data = GAMCTRP1_buffer;

    _lcd_panel_cmd[GAMCTRN1].cmd = 0xE1;
    _lcd_panel_cmd[GAMCTRN1].nrOfByte = 16;
    _lcd_panel_cmd[GAMCTRN1].data = GAMCTRN1_buffer;

    /* VCOM Control 1 */
    _lcd_panel_cmd[INVCTR].cmd = 0xB4;
    _lcd_panel_cmd[INVCTR].nrOfByte = 1;
    _lcd_panel_cmd[INVCTR].data = INVCTR_buffer;

}

void ST7735_send_sys_cmd(tST7735_system_cmd sysCmd)
{
    uint8_t i;
    /*
     * sends cmd
     * check if nrOfbyte != 0
     * if so, sends
    */
    CS_L();
    ST7735_cmd(_lcd_sys_cmd[sysCmd].cmd);
    if (_lcd_sys_cmd[sysCmd].nrOfByte != 0)
    {       
        for( i = 0 ; i < _lcd_sys_cmd[sysCmd].nrOfByte ; i++)
        {
            ST7735_data(_lcd_sys_cmd[sysCmd].data[i]);
        }
    }
    CS_H();
}

void ST7735_send_panel_cmd(tST7735_panel_cmd panelCmd)
{
    uint8_t i;
    /*
     * sends cmd
     * check if nrOfbyte != 0
     * if so, sends
    */
    CS_L();
    ST7735_cmd(_lcd_panel_cmd[panelCmd].cmd);
    if (_lcd_panel_cmd[panelCmd].nrOfByte != 0)
    {       
        for( i = 0 ; i < _lcd_panel_cmd[panelCmd].nrOfByte ; i++)
        {
            ST7735_data(_lcd_panel_cmd[panelCmd].data[i]);
        }
    }   
    CS_H();
}

/*
 // PWCTR1 Power control -4.6V, Auto mode
{ 0xC0, 0, 3, { 0xA2, 0x02, 0x84}},
// PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
{ 0xC1, 0, 1, { 0xC5}},
// PWCTR3 Power control , opamp current smal , boost frequency
{ 0xC2, 0, 2, { 0x0A, 0x00 }},
// PWCTR4 Power control , BLK/2, opamp current small and medium low
{ 0xC3, 0, 2, { 0x8A, 0x2A}},
// PWRCTR5 , VMCTR1 Power control
{ 0xC4, 0, 2, { 0x8A, 0xEE}},
{ 0xC5, 0, 1, { 0x0E }},
// INVOFF Don't invert display
{ 0x20, 0, 0, 0},
// Memory access directions. row address/col address , bottom to
,!top refesh (10.1.27)
{ ST7735_MADCTL , 0, 1, {MADVAL(MADCTLGRAPHICS)}},
// Color mode 16 bit (10.1.30
{ ST7735_COLMOD , 0, 1, {0x05}},
// Column address set 0..127
{ ST7735_CASET , 0, 4, {0x00, 0x00, 0x00, 0x7F }},
// Row address set 0..159
{ ST7735_RASET , 0, 4, {0x00, 0x00, 0x00, 0x9F }},
// GMCTRP1 Gamma correction
{ 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }},
// GMCTRP2 Gamma Polarity corrction
{ 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }},
// DISPON Display on
{ 0x29, 100, 0, 0},
// NORON Normal on
{ 0x13, 10, 0, 0},
// End
{ 0, 0, 0, 0}
 */

#define _INIT_TYPE 3

#if _INIT_TYPE  == 0
    #define _ADA_INIT
#elif _INIT_TYPE == 1
    #define _LONELY_INIT
#elif _INIT_TYPE == 2
    #define _CHINA_INIT
#else
	#define _ILI9341_INIT
#endif

void ST7735_init_with_commands(void)
{
	//CS_L();
	/* Init command */
	ST7735_sys_cmd_init();
	ST7735_panel_cmd_init();
	/* Reset Sequence START*/
	Lcd_reset();

	_lcd_screen_w = LCD_SCREEN_W;
	_lcd_screen_h = LCD_SCREEN_H;

	//CS_L();	/* start of transmission */

    ST7735_send_sys_cmd(SYS_CMD_SWRESET);	/* 0x01 */
    Delay_ms (150);

	ST7735_send_sys_cmd(SLPOUT);	/* 0x11 */
	Delay_ms (500);
	/* Reset Sequence END */
#ifndef _ADA_INIT
	/* Frame Rate */
	ST7735_send_panel_cmd(FRMCTR1);	/* 0xB1 */
	ST7735_send_panel_cmd(FRMCTR2);	/* 0xB2 */
	ST7735_send_panel_cmd(FRMCTR3);	/* 0xB3 */

	ST7735_send_panel_cmd(INVCTR);	/* 0xB4 */		/* dot inversion */
#ifdef _CHINA_INIT
	/* Power sequence */
    ST7735_send_panel_cmd(PWCTR1);	/* 0xC0 */
	ST7735_send_panel_cmd(PWCTR2);	/* 0xC1 */
	ST7735_send_panel_cmd(PWCTR3);	/* 0xC2 */
	ST7735_send_panel_cmd(PWCTR4);	/* 0xC3 */
	ST7735_send_panel_cmd(PWCTR5);	/* 0xC4 */

	ST7735_send_sys_cmd(VMCTR1);	/* 0xC5*/

	/* */
	ST7735_send_sys_cmd(INVOFF);	/* 0x20 */

	/* */
	ST7735_send_sys_cmd(MADCTL);	/* 0x36*/

	/* Color per pixel */
	//ST7735_send_sys_cmd(COLMOD);	/* 0x38 */

	/* Gamma sequence */
	ST7735_send_panel_cmd(GAMCTRP1);	/* 0xE0 */
	ST7735_send_panel_cmd(GAMCTRN1);	/* 0xE1 */

	ST7735_send_sys_cmd(IDMOFF);	/* 0x3A */

	/* column address */
	//ST7735_send_sys_cmd(CASET);		/* 0x2A */
	/* row address */
	//ST7735_send_sys_cmd(RASET);		/* 0x2B */


#elif _LONELY_INIT
    ST7735_send_sys_cmd(COLMOD);	/* 0x3A */

	ST7735_send_sys_cmd(INVOFF);		/* 0x20 */
	ST7735_send_sys_cmd(NORON);			/* 0x13 */
	ST7735_send_sys_cmd(GAMSET);			/* 0x26 */

#else   /* from here init */
	ST7735_send_panel_cmd(PWCTR1);	/* 0xC0 */
	ST7735_send_panel_cmd(PWCTR2);	/* 0xC1 */
	ST7735_send_panel_cmd(PWCTR3);	/* 0xC2 */
	ST7735_send_panel_cmd(PWCTR4);	/* 0xC3 */
	ST7735_send_panel_cmd(PWCTR5);	/* 0xC4 */

    ST7735_send_sys_cmd(VMCTR1);	/* 0xC5*/

	ST7735_send_sys_cmd(INVOFF);	/* 0x20 */

	ST7735_send_sys_cmd(MADCTL);	/* 0x36*/

	ST7735_send_sys_cmd(COLMOD);	/* 0x3A */

	//ST7735_send_panel_cmd(GAMCTRP1);	/* 0xE0 */
	//ST7735_send_panel_cmd(GAMCTRN1);	/* 0xE1 */
	ST7735_send_panel_cmd(CASET);	/* 0xE0 */
	ST7735_send_panel_cmd(RASET);	/* 0xE1 */
	ST7735_send_panel_cmd(GAMCTRP1);	/* 0xE0 */
	ST7735_send_panel_cmd(GAMCTRN1);	/* 0xE1 */
#endif
#endif
	ST7735_send_sys_cmd(NORON);		/* */
	Delay_ms(10);

	ST7735_send_sys_cmd(DISPON);	/* */
	Delay_ms(100);

	//CS_H();	/* end of transmission */
}

void ST7735_turns_display(uint8_t power)
{
	CS_L();	/* start of transmission */
	if (SET == power)
	{
		ST7735_send_sys_cmd(DISPON);
	}
	else
	{
		ST7735_send_sys_cmd(DISPOFF);
	}
	CS_H();	/* end of transmission */
}

void ST7735_set_orientation(tLCD_orientation orientation)
{
    CS_L();
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

void ST7735_set_windows(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t tempVal = 0;

    if (x0 >= LCD_SCREEN_W)
    {
        x0 = LCD_SCREEN_W - 1;
    }
    if (x1 >= LCD_SCREEN_W)
    {
        x1 = LCD_SCREEN_W - 1;
    }

    if (y0 >= LCD_SCREEN_H)
    {
        y0 = LCD_SCREEN_H - 1;
    }

    if (y1 >= LCD_SCREEN_H)
    {
        y1 = LCD_SCREEN_H - 1;
    }

    if (x0 > x1)
    {
        tempVal = x1;
        x1 = x0;
        x0 = tempVal;
    }

    if (y0 > y1)
    {
        tempVal = y1;
        y1 = y0;
        y0 = tempVal;
    }

    //CASET_buffer_buffer[0] = (x0>>8);
    CASET_buffer_buffer[0] = (0x00);
    CASET_buffer_buffer[1] = (x0 & 0xff);
    //CASET_buffer_buffer[2] = (x1>>8);
    CASET_buffer_buffer[2] = (0x00);
    CASET_buffer_buffer[3] = (x1 & 0xff);

    //RASET_buffer_buffer[0] = (y0>>8);
    RASET_buffer_buffer[0] = (0x00);
    RASET_buffer_buffer[1] = (y0 & 0xff);
    //RASET_buffer_buffer[2] = (y1>>8);
    RASET_buffer_buffer[2] = (0x00);
    RASET_buffer_buffer[3] = (y1 & 0xff);

    // CS_L();    
	ST7735_send_sys_cmd(CASET);		/* 0x2A */
	ST7735_send_sys_cmd(RASET);		/* 0x2B */    

	ST7735_send_sys_cmd(RAMWR);		/* 0x2C */

    // CS_H();
}

void ST7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    CS_L();
    ST7735_set_windows(x, y, x, y);    
    A0_H();
    ST7735_write(color >> 8);
    ST7735_write((uint8_t)color);
    CS_H();
}

void ST7735_draw_filled_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint8_t i;
    uint8_t j;

    //CS_L();
    ST7735_set_windows(x0, y0, x1, y1);

    A0_H();
    CS_L();

    for (i = x0; i < x1; i++) 
    {
        for (j = y0; j < y1; j++) 
        {
            ST7735_write(color >> 8);
            ST7735_write((uint8_t)color);
        }
    }

    CS_H();
}

void ST7735_draw_unfilled_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	ST7735_draw_v_line(x0, y0, y1, color);
	ST7735_draw_v_line(x1, y0, y1, color);

	ST7735_draw_h_line(x0, x1, y0, color);
	ST7735_draw_h_line(x0, x1, y1, color);
}


void ST7735_draw_h_line(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color)
{
    ST7735_draw_filled_rectangle(x0, y, x1, y, color);
}

void ST7735_draw_v_line(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color)
{
    ST7735_draw_filled_rectangle(x, y0, x, y1, color);
}

void ST7735_clear(uint16_t color)
{
    ST7735_draw_filled_rectangle(0, 0, (LCD_SCREEN_W - 1), (LCD_SCREEN_H - 1), color);
}
