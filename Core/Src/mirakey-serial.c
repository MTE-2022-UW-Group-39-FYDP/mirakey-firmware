#include "mirakey-serial.h"

/*-------------------------------------------------------------------------------------------------
Helper Method Prototypes
-------------------------------------------------------------------------------------------------*/
void MKS_ActivateDisplay(uint8_t SlaveAddress, uint8_t * pDispBuf);
void MKS_InitializationCmds(uint8_t height, uint8_t width, uint8_t hrz_offset);
void MKS_Select(uint8_t SlaveAddress);
void MKS_TxCommand(uint8_t* pCommand, size_t Size);
void MKS_TxData(uint8_t * pData, size_t Size);

/*-------------------------------------------------------------------------------------------------
Mirakey Serial Interface
-------------------------------------------------------------------------------------------------*/

SPI_HandleTypeDef * MKS_SPI_PORT;
UART_HandleTypeDef * MKS_UART_PORT;	// for debugging

void MKS_Init(SPI_HandleTypeDef * spi_port, UART_HandleTypeDef * uart_port) {
	uint8_t blank_screen[MKS_DISP_WIDTH * MKS_DISP_HEIGHT / 8] = {
			[0 ... sizeof blank_screen - 1] = 0x00
	};

	MKS_SPI_PORT = spi_port;
	MKS_UART_PORT = uart_port;

	// reset sequence for SSD1306 requires RESET to go low then high
    HAL_GPIO_WritePin(MKS_RESET_PORT, MKS_RESET_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(MKS_RESET_PORT, MKS_RESET_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_Delay(100);
    // run initialization commands for all displays
    //for(uint8_t slaveAddress = 0; slaveAddress<MKS_NUM_KEYS; slaveAddress++) {
    //}

    // for breadboard testing
    MKS_ActivateDisplay(0b11111110, blank_screen);
    HAL_Delay(100);
	MKS_ActivateDisplay(0b11111101, blank_screen);
	HAL_Delay(100);

}

void MKS_TxGlyph(uint8_t SlaveAddress, uint8_t* GlyphBuffer) {
    MKS_Select(SlaveAddress);
    MKS_TxData(GlyphBuffer,40 * 40 / 8);
}

/*-------------------------------------------------------------------------------------------------
Helper Methods
-------------------------------------------------------------------------------------------------*/

void MKS_InitializationCmds(uint8_t height, uint8_t width, uint8_t hrz_offset) {
	uint8_t init_cmds[] = {
		0xAE,	// Turn display off

		0x20,	// Set Memory Addressing Mode
		0x00,	// Horizontal Addressing Mode

		0xB0,

		0x21,	// Set the start and end columns
		(uint8_t)(hrz_offset),
		(uint8_t)(hrz_offset + width - 1),

		0x22,	// Set start and end pages
		0x00,
		(uint8_t)(height/8 - 1),	// 5 pages for 40px height

		0xC8,	// Set COM output scan direction <--- vertical orientation (CHECK) alt is 0xC0

		0x40, 	// Set start line address - default

		0x81,   // Set contrast
		0xFF,   // to maximum

		0xA1,   // Set segment re-map 0 to 127 - default <--- horizontal orientation

		0xA6,   // Set normal color

		0xA8,   // Set multiplex ratio
		(uint8_t)(height - 1),   // value depends on screen height

		0xA4,   // Make output track RAM content (0xA5 turns everything on ignoring RAM)

		0xD3,   // Set display offset - default
		0x00,   // offset = 0

		0xD5,   // Set display clock divide ratio/oscillator frequency - default
		0xF0,   // ratio = 1

		0xD9,   // Set pre-charge period
		0x22,   //

		0xDA,   // Set com pins hardware configuration - CHECK
		0x12,   // related to height

		0xDB,   // Set vcomh
		0x20,   // 0x20,0.77xVcc

		0x8D,   // Set DC-DC enable . charge pump
		0x14,   //

		0xAF,   // Turn on SSD1306 panel

		//0xA5,	// used to test display
	};
	MKS_TxCommand(init_cmds, 32);
}
void MKS_ActivateDisplay(uint8_t SlaveAddress, uint8_t * pDispBuf) {
	MKS_Select(SlaveAddress);
	MKS_InitializationCmds(MKS_DISP_HEIGHT, MKS_DISP_WIDTH, MKS_DISP_HRZ_OFFSET);
	HAL_Delay(10);
	MKS_TxData(pDispBuf, MKS_DISP_WIDTH * MKS_DISP_HEIGHT / 8);	// blank screen
	MKS_InitializationCmds(40, 40, MKS_DISP_HRZ_OFFSET + 16); // adjust to glyph size
	HAL_Delay(10);
}

void MKS_Select(uint8_t SlaveAddress) {
#ifdef MKS_SS_BITBANG
	// SA slave address
	for(uint8_t i=0; i<8; i++) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_RESET);

		HAL_Delay(1);

		HAL_GPIO_WritePin(MKS_SSA_PORT,MKS_SSA_PIN,((SlaveAddress & (0x80>>i)) != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET); // set SS bit, MSB first

		HAL_Delay(1);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_SET);

		HAL_Delay(1);
	}

	// SAP, slave address prefix guarantees CS update
	HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_RESET); 			// SS clock down
	HAL_Delay(1);
	HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_SET); 				// SS clock up
	HAL_Delay(1);//*/
#endif
#ifdef MKS_SS_CL_SYNC
	HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_SET);			// slave select enable
	HAL_SPI_Transmit(MKS_SPI_PORT, &SlaveAddress, 1, HAL_MAX_DELAY);	// transmit slave address
	HAL_GPIO_WritePin(MKS_SSC_PORT,MKS_SSC_PIN,GPIO_PIN_RESET);			// slave select disable
#endif
}

void MKS_TxCommand(uint8_t* pCommand, size_t Size) {
	HAL_GPIO_WritePin(MKS_DC_PORT,MKS_DC_PIN,GPIO_PIN_RESET); 		// D/C# command low
	HAL_SPI_Transmit(MKS_SPI_PORT, pCommand, Size, HAL_MAX_DELAY);
}

void MKS_TxData(uint8_t * pData, size_t Size) {
	HAL_GPIO_WritePin(MKS_DC_PORT,MKS_DC_PIN,GPIO_PIN_SET);			// D/C# data high
	HAL_SPI_Transmit(MKS_SPI_PORT, pData, Size, HAL_MAX_DELAY);
}
