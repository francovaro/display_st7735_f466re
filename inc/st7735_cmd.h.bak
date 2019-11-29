/*
 *  @file  : st7735_cmd.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 27 nov 2019
 */

#ifndef ST7735_CMD_H_
#define ST7735_CMD_H_

typedef struct
{
	uint8_t cmd;
	uint8_t *data;
	uint8_t nrOfByte;
} tLCD_cmd;

typedef enum
{
	NOP,
	SWRESET,
	RDDID,
	RDDST,
	RDDPM,
	RDD_MADCTL,
	RDD_COLMOD,
	RDDIM,
	RDDSM,
	RDDSDR,
	SLPIN,
	SLPOUT,
	PTLON,
	NORON,
	INVOFF,
	INVON,
	GAMSET,
	DISPOFF,
	DISPON,
	CASET,
	RASET,
	RAMWR,
	RGBSET,
	RAMRD,
	PTLAR,
	SCRLAR,
	TEOFF,
	TEON,
	MADCTL,
	VSCSAD,
	IDMOFF,
	IDMON,
	COLMOD,
	RDID1,
	RDID2,
	RDID3
}tST7735_system_cmd;

typedef enum
{
	FRMCTR1,
	FRMCTR2,
	FRMCTR3,
	INVCTR,
	PWCTR1,
	PWCTR2,
	PWCTR3,
	PWCTR4,
	PWCTR5,
	VMCTR1,
	VMOFCTR,
	WRID2,
	WRID3,
	NVCTR1,
	NVCTR2,
	NVCTR3
}tST7735_panel_cmd;

#endif /* ST7735_CMD_H_ */
