/*
Mirakey Serial Protocol
by Francisco Garcia-Gonzalez
University of Waterloo Undergraduate Mechatronics Engineering - W2022
MTE482 Fourth Year Design Project: Mirakey
*/

#include "main.h"



/* HAL Equivalent */
void MKS_Init();
void MKS_Select(uint8_t SlaveAddress);
void MKS_TxCommand(uint8_t Command);
void MKS_TxData(uint8_t * pData, uint16_t Size);

/* Helper Methods */
static void MKS_GPIO_Init(void);
void MKS_ActivateDisplay(uint8_t SlaveAddress);
}