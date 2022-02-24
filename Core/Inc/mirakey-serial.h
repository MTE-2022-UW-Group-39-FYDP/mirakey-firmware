/*
Mirakey Serial Protocol
by Francisco Garcia-Gonzalez
University of Waterloo Undergraduate Mechatronics Engineering - W2022
MTE482 Fourth Year Design Project: Mirakey
*/

#include "main.h"

#ifndef MKS_h
#define MKS_h

#define MKS_RESET_PORT GPIOA
#define MKS_RESET_PIN GPIO_PIN_10 //might need to set this to something else

#define MKS_NUM_KEYS 1
/* Layout of the 12 key macro-pad with slave addresses in hex

+---+---+---+---+
| 00| 01| 02| 03|
+---+---+---+---+
| 04| 05| 06| 07|
+---+---+---+---+
| 08| 09| 0A| 0B|
+---+---+---+---+

*/
#define MKS_DISP_CONTROLLER_SSD1306
#define MKS_DISP_HEIGHT 40
#define MKS_DISP_WIDTH 70


/* Mirakey Serial Interface */
void MKS_Init();
void MKS_TxGlyph(uint8_t SlaveAddress, uint8_t * GlyphBuffer);

#endif
