/*
 *  @file  : spi.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 25 nov 2019
 */
#ifndef SPI_H_
#define SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define SPIx                           SPI2
#define SPIx_CLK                       RCC_APB1Periph_SPI2
//#define SPIx_CLK                       RCC_APB2Periph_SPI1
#define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
//#define SPIx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define SPIx_IRQn                      SPI2_IRQn
#define SPIx_IRQHANDLER                SPI2_IRQHandler

/* CLK - GPIOB 10*/
#define SPIx_SCK_PIN                   GPIO_Pin_7
#define SPIx_SCK_GPIO_PORT             GPIOC
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define SPIx_SCK_SOURCE                GPIO_PinSource7
#define SPIx_SCK_AF                    GPIO_AF_SPI2

#if 0
#define SPIx_MISO_PIN                  GPIO_Pin_2
#define SPIx_MISO_GPIO_PORT            GPIOC
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPIx_MISO_SOURCE               GPIO_PinSource2
#define SPIx_MISO_AF                   GPIO_AF_SPI2
#endif

/* MOSI/SDA -  GPIOC 1*/
#define SPIx_MOSI_PIN                  GPIO_Pin_3
#define SPIx_MOSI_GPIO_PORT            GPIOC
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPIx_MOSI_SOURCE               GPIO_PinSource3
#define SPIx_MOSI_AF                   GPIO_AF_SPI2

/* CHIP SELECT - GPIOB 12*/
#define SPIx_CS_PIN                    GPIO_Pin_2
#define SPIx_CS_GPIO_PORT              GPIOC
#define SPIx_CS_GPIO_CLK               RCC_AHB1Periph_GPIOC

/* A0 - GPIOC 3*/
#define LCD_A0_PIN                     GPIO_Pin_0
#define LCD_A0_GPIO_PORT               GPIOC
#define LCD_A0_GPIO_CLK                RCC_AHB1Periph_GPIOC

/* RESET - GPIOC 4*/
#define LCD_RESET_PIN                  GPIO_Pin_4
#define LCD_RESET_GPIO_PORT            GPIOC
#define LCD_RESET_GPIO_CLK             RCC_AHB1Periph_GPIOC

// CS pin macros
#define CS_L() GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define CS_H() GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

// A0 pin macros
#define A0_L() GPIO_ResetBits(LCD_A0_GPIO_PORT, LCD_A0_PIN)
#define A0_H() GPIO_SetBits(LCD_A0_GPIO_PORT, LCD_A0_PIN)

// RESET pin macros
#define RST_L() GPIO_ResetBits(LCD_RESET_GPIO_PORT, LCD_RESET_PIN)
#define RST_H() GPIO_SetBits(LCD_RESET_GPIO_PORT, LCD_RESET_PIN)

#define BUFFERSIZE                     100	/* */

void SPI_Config(void);
void SPIx_IRQHANDLER(void);

#endif /* SPI_H_ */
