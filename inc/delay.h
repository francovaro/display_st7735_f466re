/*
 *  @file  : delay.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 26 nov 2019
 */

#ifndef DELAY_H_
#define DELAY_H_

__IO uint32_t TimingDelay; // __IO -- volatile


/*
 *   Declare Functions
 */
extern void Delay_ms(uint32_t nTime);
extern void Delay_us(uint32_t nTime);


#endif /* DELAY_H_ */
