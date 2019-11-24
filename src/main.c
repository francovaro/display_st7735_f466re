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

int main(void)
{
	const char strStart[] = "START\n";

	UART_fv_config(RESET);

	UART_fv_SendData(strStart, strlen(strStart));

	while(1)
	{

	}

}
