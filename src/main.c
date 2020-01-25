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
#include "color.h"
#include "st7735_cmd.h"
#include "delay.h"

int main(void)
{
	uint16_t color = 0;
	const char strStart[] = "START\n";

	UART_fv_config(RESET);
	SPI_Config();

	ST7735_init_with_commands();
	//ST7735_set_orientation(eLCD_orientation_normal);
	ST7735_clear(0);

	UART_fv_SendData(strStart, strlen(strStart));

	while(1)
	{
		/*
		ST7735_clear(color);
		Delay_ms(250);
		ST7735_draw_filled_rectangle(20, 20, 60, 60, !(color));
		Delay_ms(250);
		color++;
		*/
		ST7735_turns_display(0);
		Delay_ms(500);
		ST7735_turns_display(1);
		Delay_ms(500);
		//__WFI();
	}


	return 0;
}
