/*
Mirakey Serial Protocol
by Francisco Garcia-Gonzalez
University of Waterloo Undergraduate Mechatronics Engineering - W2022
MTE482 Fourth Year Design Project: Mirakey
*/

#include "main.h"

#ifndef MKS
#define MKS

#define MKS_NUM_KEYS 12
#define MKS_SPI_PORT hspi2
#define MKS_DISP_CONTROLLER_SSD1306
#define MKS_DISP_HEIGHT 40
#define MKS_DISP_WIDTH 70


/* Mirakey Serial Interface */
void MKS_Init();
void MKS_TxGlyph(uint8_t SlaveAddress, uint8_t* GlyphBuffer);

#endif

/*
Notes on firmware design


start the DMA

prepare memory to be transfered

one call to MKS for every memory section

MKS begins

CPU is independent at this point

MKS runs through the transaction memory

*/
