/*
 *  @file  : delay.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 26 nov 2019
 */

#include "systick.h"

#include "delay.h"

#define DELAY_TICK_FREQUENCY_US 1000000   /* = 1MHZ -> microseconds delay */
#define DELAY_TICK_FREQUENCY_MS 1000      /* = 1kHZ -> milliseconds delay */

// Do delay for mSecs milliseconds
/**
 *
 */
void Delay_ms(uint32_t mSecs)
{
	setSysTick(DELAY_TICK_FREQUENCY_MS);
	TimingDelay = mSecs+1;
	while (TimingDelay != 0);
}

// Do delay for nSecs microseconds
/**
 *
 */
void Delay_us(uint32_t uSecs)
{
	setSysTick(DELAY_TICK_FREQUENCY_US);
	TimingDelay = uSecs+1;
	while (TimingDelay != 0);
}
