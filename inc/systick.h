/*
 * systick.h
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f4xx.h"

uint8_t interruptSys;
extern void setSysTick(uint32_t timeMs);

#endif /* SYSTICK_H_ */
