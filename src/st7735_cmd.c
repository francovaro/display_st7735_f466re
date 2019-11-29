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

#define NULL 0;

static tLCD_cmd _lcd_sys_cmd[e_syscmd_max];
static tLCD_cmd _lcd_panel_cmd[e_panelcmd_max];

static uint8_t FRMCTR1_buffer[3] = { 0x01, 0x2C, 0x2D};
static uint8_t FRMCTR2_buffer[3] = { 0x01, 0x2C, 0x2D};
static uint8_t FRMCTR3_buffer[6] = { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D};

static uint8_t PWCTR1_buffer[3] = { 0xA2, 0x02, 0x84};
static uint8_t PWCTR2_buffer[1] = { 0xC5};
static uint8_t PWCTR3_buffer[2] = { 0x0A, 0x00};
static uint8_t PWCTR4_buffer[2] = { 0x8A, 0x2A};
static uint8_t PWCTR5_buffer[2] = { 0x8A, 0xEE};
static uint8_t VMCTR1_buffer[1] = { 0x0E};
static uint8_t GAMCTRP1_buffer[16] = { 0x0E};
static uint8_t GAMCTRN1_buffer[16] = { 0x0E};

static uint8_t CASET_buffer_buffer[4] = {0x00, 0x00, 0x00, 0x7F};
static uint8_t RASET_buffer_buffer[4] = {0x00, 0x00, 0x00, 0x9F};

void ST7735_sys_cmd_init(void)
{
    /* Sleep Out & Booster On */
    _lcd_sys_cmd[SLPOUT].cmd = 0x11;
    _lcd_sys_cmd[SLPOUT].nrOfByte = 0;
    _lcd_sys_cmd[SLPOUT].data = NULL;

    /* Memory Data Access Control */
    _lcd_sys_cmd[MADCTL].cmd = 0x36;
    _lcd_sys_cmd[MADCTL].nrOfByte = 1;
    _lcd_sys_cmd[MADCTL].data = NULL;
    
    /* Column Address Set */
    _lcd_sys_cmd[CASET].cmd = 0x2A;
    _lcd_sys_cmd[CASET].nrOfByte = 4;
    _lcd_sys_cmd[CASET].data = CASET_buffer_buffer;

    /* Row Address Set */
    _lcd_sys_cmd[RASET].cmd = 0x2B;
    _lcd_sys_cmd[RASET].nrOfByte = 4;
    _lcd_sys_cmd[RASET].data = RASET_buffer_buffer;
}


void ST7735_panel_cmd_init(void)
{
    /* In Normal Mode (Full Colors) */
    _lcd_sys_cmd[FRMCTR1].cmd = 0xB1;
    _lcd_sys_cmd[FRMCTR1].nrOfByte = 3;
    _lcd_sys_cmd[FRMCTR1].data = FRMCTR1_buffer;

    /* In Idle Mode (8-colors) */
    _lcd_sys_cmd[FRMCTR2].cmd = 0xB2;
    _lcd_sys_cmd[FRMCTR2].nrOfByte = 3;
    _lcd_sys_cmd[FRMCTR2].data = FRMCTR2_buffer;

    /* In Partial Mode + FullColors */
    _lcd_sys_cmd[FRMCTR3].cmd = 0xB3;
    _lcd_sys_cmd[FRMCTR3].nrOfByte = 6;
    _lcd_sys_cmd[FRMCTR3].data = FRMCTR3_buffer;

    /*  Power Control Setting */
    _lcd_sys_cmd[PWCTR1].cmd = 0xC0;
    _lcd_sys_cmd[PWCTR1].nrOfByte = 3;
    _lcd_sys_cmd[PWCTR1].data = PWCTR1_buffer;

    /* Power Control Setting */
    _lcd_sys_cmd[PWCTR2].cmd = 0xC1;
    _lcd_sys_cmd[PWCTR2].nrOfByte = 1;
    _lcd_sys_cmd[PWCTR2].data = PWCTR2_buffer;

    _lcd_sys_cmd[PWCTR3].cmd = 0xC2;
    _lcd_sys_cmd[PWCTR3].nrOfByte = 2;
    _lcd_sys_cmd[PWCTR3].data = PWCTR3_buffer;

    _lcd_sys_cmd[PWCTR4].cmd = 0xC3;
    _lcd_sys_cmd[PWCTR4].nrOfByte = 2;
    _lcd_sys_cmd[PWCTR4].data = PWCTR4_buffer;

    _lcd_sys_cmd[PWCTR5].cmd = 0xC3;
    _lcd_sys_cmd[PWCTR5].nrOfByte = 2;
    _lcd_sys_cmd[PWCTR5].data = PWCTR5_buffer;

    /* VCOM Control 1 */
    _lcd_sys_cmd[VMCTR1].cmd = 0xC5;
    _lcd_sys_cmd[VMCTR1].nrOfByte = 1;
    _lcd_sys_cmd[VMCTR1].data = VMCTR1_buffer;

    _lcd_sys_cmd[GAMCTRP1].cmd = 0xE0;
    _lcd_sys_cmd[GAMCTRP1].nrOfByte = 16;
    _lcd_sys_cmd[GAMCTRP1].data = GAMCTRP1_buffer;

    _lcd_sys_cmd[GAMCTRN1].cmd = 0xE1;
    _lcd_sys_cmd[GAMCTRN1].nrOfByte = 16;
    _lcd_sys_cmd[GAMCTRN1].data = GAMCTRN1_buffer;
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

void ST7735_init_with_commands(void)
{
	ST7735_sys_cmd_init();
	void ST7735_panel_cmd_init();
	CS_L();
}

