/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"

#include <string.h>
			
#include "systick.h"
#include "uart.h"
#include "spi.h"
#include "lcd.h"
#include "color.h"
#include "st7735_cmd.h"
#include "delay.h"

int main(void)
{
	const char strStart[] = "START\n";

	UART_fv_config(RESET);
	SPI_Config();

	ST7735_init_with_commands();

	CS_L();
	ST7735_AddrSet(0, 0, (LCD_SCREEN_W-1), (LCD_SCREEN_H-1));
	ST7735_Clear(COLOR_BLUE);
	CS_H();
	//ST7735_turns_display(0);

	UART_fv_SendData(strStart, strlen(strStart));

	while(1)
	{
		//ST7735_Clear(COLOR_BLUE);
		CS_L();
		ST7735_Clear(COLOR_BLUE);
		Delay_ms(500);
		CS_H();
		Delay_ms(500);
	}

}
