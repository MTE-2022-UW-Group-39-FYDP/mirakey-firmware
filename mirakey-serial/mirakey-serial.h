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
#define SSD1306_SPI_PORT hspi2
#define SSD1306_HEIGHT 40
#define SSD1306_WIDTH 72

/* HAL Equivalent */
void MKS_Init();
void MKS_Select(uint8_t SlaveAddress);
void MKS_TxCommand(uint8_t Command);
void MKS_TxData(uint8_t * pData, uint16_t Size);

/* Helper Methods */
void MKS_ActivateDisplay(uint8_t SlaveAddress);
void MKS_GPIO_Init(void);
void MKS_ActivateDisplay(uint8_t SlaveAddress);
}

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
