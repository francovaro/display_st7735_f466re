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
  #define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define SPIx_IRQn                      SPI2_IRQn
  #define SPIx_IRQHANDLER                SPI2_IRQHandler

  #define SPIx_SCK_PIN                   GPIO_Pin_10
  #define SPIx_SCK_GPIO_PORT             GPIOB
  #define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
  #define SPIx_SCK_SOURCE                GPIO_PinSource10
  #define SPIx_SCK_AF                    GPIO_AF_SPI2

  #define SPIx_MISO_PIN                  GPIO_Pin_2
  #define SPIx_MISO_GPIO_PORT            GPIOC
  #define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
  #define SPIx_MISO_SOURCE               GPIO_PinSource2
  #define SPIx_MISO_AF                   GPIO_AF_SPI2

  #define SPIx_MOSI_PIN                  GPIO_Pin_3
  #define SPIx_MOSI_GPIO_PORT            GPIOC
  #define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
  #define SPIx_MOSI_SOURCE               GPIO_PinSource3
  #define SPIx_MOSI_AF                   GPIO_AF_SPI2

  #define SPIx_CS_PIN                  GPIO_Pin_12
  #define SPIx_CS_GPIO_PORT            GPIOB
  #define SPIx_CS_GPIO_CLK             RCC_AHB1Periph_GPIOB
  #define SPIx_CS_SOURCE               GPIO_PinSource12
  #define SPIx_CS_AF                   GPIO_AF_SPI2
  #define SPIx_CS_IRQHANDLER 		   EXTI0_IRQn

#define BUFFERSIZE                       100	/* */

volatile uint8_t spi_dataReceived;
volatile uint8_t spi_nextDR;
__IO uint8_t spi_ubRxIndex;
__IO uint8_t spi_RxBuffer [BUFFERSIZE];
volatile uint8_t spi_txBuffer [BUFFERSIZE];


void SPI_Config(void);
void SPIx_IRQHANDLER(void);

#endif /* SPI_H_ */
