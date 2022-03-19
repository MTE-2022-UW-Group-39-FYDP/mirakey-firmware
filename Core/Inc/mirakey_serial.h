/*--------------------------------------------------------------------------------------------
 * Mirakey Serial Protocol
 * by Francisco Garcia-Gonzalez
 * University of Waterloo Undergraduate Mechatronics Engineering - W2022
 * MTE482 Fourth Year Design Project Group 39: Mirakey
 * -----------------------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "stm32f412rx.h"

#ifndef MKS_h
#define MKS_h
/*--------------------------------------------------------------------------------------------
 * Mirakey Serial I/O Configuration
 * -----------------------------------------------------------------------------------------*/
//#define MKS_SPI_PORT    hspi2		// we're using a variable for this
#define MKS_RESET_PORT	GPIOB
#define MKS_RESET_PIN	GPIO_PIN_7
#define MKS_DC_PORT		GPIOB
#define MKS_DC_PIN		GPIO_PIN_6

// Slave Select Modes: Bitbang, ClockSync
//#define MKS_SS_BITBANG		// The slave selection is manually driven with GPIO pins.
//#define MKS_SS_CL_SYNC	// The slave selection is enabled/disabled with MKS_SS_PIN.
							// Requires hardware to sync data and address clock lines.
#define MKS_SPI           // Individual chip selects as in regular SPI.
                            // This is only recommended for macropads with few keys.

#ifdef MKS_SS_BITBANG
#define MKS_SSA_PORT 	GPIOA
#define MKS_SSA_PIN		GPIO_PIN_10
#define MKS_SSC_PORT 	GPIOC
#define MKS_SSC_PIN		GPIO_PIN_7
#endif
#ifdef MKS_SS_CL_SYNC
#define MKS_SS_PORT		GPIOA
#define MKS_SS_PIN		GPIO_PIN_9
#endif
#ifdef MKS_SPI
#define MKS_CS0_PORT    GPIOB
#define MKS_CS0_PIN     GPIO_PIN_2
#define MKS_CS1_PORT    GPIOB
#define MKS_CS1_PIN     GPIO_PIN_13
#define MKS_CS2_PORT    GPIOC
#define MKS_CS2_PIN     GPIO_PIN_10
#define MKS_CS3_PORT    GPIOC
#define MKS_CS3_PIN     GPIO_PIN_12
#define MKS_CS4_PORT    GPIOB
#define MKS_CS4_PIN     GPIO_PIN_15
#define MKS_CS5_PORT    GPIOC
#define MKS_CS5_PIN     GPIO_PIN_7
#define MKS_CS6_PORT    GPIOA
#define MKS_CS6_PIN     GPIO_PIN_9
#define MKS_CS7_PORT    GPIOB
#define MKS_CS7_PIN     GPIO_PIN_4
#define MKS_CS8_PORT    GPIOC
#define MKS_CS8_PIN     GPIO_PIN_14
#define MKS_CS9_PORT    GPIOC
#define MKS_CS9_PIN     GPIO_PIN_14
#define MKS_CS10_PORT   GPIOC
#define MKS_CS10_PIN    GPIO_PIN_14
#define MKS_CS11_PORT   GPIOC
#define MKS_CS11_PIN    GPIO_PIN_14
#endif


/*--------------------------------------------------------------------------------------------
 * Mirakey Serial Peripherals Configuration
 * -----------------------------------------------------------------------------------------*/

#define MKS_NUM_KEYS 12
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
#define MKS_DISP_WIDTH 72
#define MKS_DISP_HRZ_OFFSET 0x1C
//#define MKS_DISP_MUX_RATIO 0xA8


/*--------------------------------------------------------------------------------------------
 * Mirakey Serial Programming Interface
 * -----------------------------------------------------------------------------------------*/
void MKS_Init();
void MKS_TxGlyph(uint8_t SlaveAddress, uint8_t * GlyphBuffer);

#endif
