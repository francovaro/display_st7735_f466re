/*
 * systick.c
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */

#include "systick.h"

#define SYSTICK_LIMIT	100

extern __IO uint32_t TimingDelay; // __IO -- volatile

void setSysTick(uint32_t timeMs)
{
	 RCC_ClocksTypeDef RCC_Clocks;
	 RCC_GetClocksFreq(&RCC_Clocks);
	 SysTick_Config(RCC_Clocks.HCLK_Frequency/timeMs);

	 interruptSys = RESET;
}

void SysTick_Handler(void)
{
	/*
	static uint16_t counter = 0;

	counter++;

	if(counter > SYSTICK_LIMIT)
	{
		interruptSys = SET;
		counter = 0;
	}
	*/
	if (TimingDelay != 0)
	{
		TimingDelay--;
	}
}
