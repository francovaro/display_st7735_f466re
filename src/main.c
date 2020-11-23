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
			
#include "lib_systick.h"
#include "lib_uart.h"
#include "spi.h"
#include "color.h"
#include "st7735_cmd.h"
#include "delay.h"

int main(void)
{
	uint16_t color = 0;
	char strStart[] = "START\n";

	Delay_init();
	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	SPI_Config();

	ST7735_init_with_commands();
	ST7735_set_orientation(eLCD_orientation_normal);
	//ST7735_set_windows(00, 00, 127, 127);
	ST7735_turns_display(SET);
	ST7735_clear(ST7735_WHITE);

	UART_lib_sendData(strStart, strlen(strStart));

	while(1)
	{
		ST7735_clear(ST7735_ORANGE);
		Delay_ms(500);
		ST7735_clear(ST7735_MAGENTA);
		Delay_ms(500);

		//color++;
		/*
		ST7735_turns_display(0);
		Delay_ms(500);
		ST7735_turns_display(1);
		Delay_ms(500);
		*/
		//__WFI();
	}


	return 0;
}
