#include "mirakey-serial.h"

/*-------------------------------------------------------------------------------------------------
Helper Method Prototypes
-------------------------------------------------------------------------------------------------*/
void MKS_ActivateDisplay(uint8_t SlaveAddress);
void MKS_Select(uint8_t SlaveAddress);
void MKS_TxCommand(uint8_t Command);
void MKS_TxData(uint8_t * pData, uint16_t Size);

/*-------------------------------------------------------------------------------------------------
Mirakey Serial Interface
-------------------------------------------------------------------------------------------------*/

SPI_HandleTypeDef * MKS_SPI_PORT;

void MKS_Init(SPI_HandleTypeDef * spi_port) {
	MKS_SPI_PORT = spi_port;

    HAL_GPIO_WritePin(MKS_RESET_PORT, MKS_RESET_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(MKS_RESET_PORT, MKS_RESET_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    HAL_Delay(100);

    for(uint8_t slaveAddress = 0; slaveAddress<MKS_NUM_KEYS; slaveAddress++) {
    	MKS_ActivateDisplay(slaveAddress);
    }
}

void MKS_TxGlyph(uint8_t SlaveAddress, uint8_t* GlyphBuffer) {
    MKS_Select(SlaveAddress);
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < MKS_DISP_HEIGHT/8; i++) {
        MKS_TxCommand(0xB0 + i); // Set the current RAM page address.
        MKS_TxCommand(0x00);
        MKS_TxCommand(0x10);
        MKS_TxData(&GlyphBuffer[MKS_DISP_WIDTH*i],MKS_DISP_WIDTH);
    }
}

/*-------------------------------------------------------------------------------------------------
Helper Methods
-------------------------------------------------------------------------------------------------*/

void MKS_ActivateDisplay(uint8_t SlaveAddress) {

    MKS_Select(SlaveAddress);  

    MKS_TxCommand(0xAE);  // turn it off

    MKS_TxCommand(0x20);  //Set Memory Addressing Mode
    MKS_TxCommand(0x00);  // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                          // 10b,Page Addressing Mode (RESET); 11b,Invalid

    MKS_TxCommand(0xB0);  //Set Page Start Address for Page Addressing Mode,0-7
    MKS_TxCommand(0xC8);  // Set COM output scan direction
    MKS_TxCommand(0x00);  //---set low column address
    MKS_TxCommand(0x10);  //---set high column address
    MKS_TxCommand(0x40);  //--set start line address - CHECK

    MKS_TxCommand(0x81);  // set max contrast
    MKS_TxCommand(0xFF);  // set max contrast

    MKS_TxCommand(0xA1);  // set segment re-map 0 to 127 - CHECK
    MKS_TxCommand(0xA6);  // set normal color

    MKS_TxCommand(0xA8);  // set multiplex ratio
    MKS_TxCommand(0x3F);  // set height
    MKS_TxCommand(0xA4);  //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    MKS_TxCommand(0xD3);  //-set display offset - CHECK
    MKS_TxCommand(0x00);  //-not offset

    MKS_TxCommand(0xD5);  //--set display clock divide ratio/oscillator frequency
    MKS_TxCommand(0xF0);  //

    MKS_TxCommand(0xD9);  //--set pre-charge period
    MKS_TxCommand(0x22);  //

    MKS_TxCommand(0xDA);  //--set com pins hardware configuration - CHECK
    #if (MKS_DISP_HEIGHT  == 40)
    MKS_TxCommand(0x12);  // related to HEIGHT
	#else
	#error(Unsupported display dimmensions)
    #endif

    MKS_TxCommand(0xDB);  //--set vcomh
    MKS_TxCommand(0x20);  //0x20,0.77xVcc

    MKS_TxCommand(0x8D);  //--set DC-DC enable . charge pump
    MKS_TxCommand(0x14);  //

    MKS_TxCommand(1);     //--turn on SSD1306 panel
}

void MKS_Select(uint8_t SlaveAddress) {
	// actual MKS Select
    // todo: sync the register clock with SPI clock
    //HAL_SPI_Transmit(&MKS_SPI_PORT, (uint8_t *)SlaveAddress, sizeof(tx), 5000);
    // todo: desync register clock

	// temporary test code
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

}

void MKS_TxCommand(uint8_t Command) {
	//*/ HSPI 16-BIT
	uint8_t word[2];
	word[0] = 0x00 | (Command>>1);
	word[1] = Command<<7;
    HAL_SPI_Transmit(MKS_SPI_PORT, &word, 2, HAL_MAX_DELAY);
	//*/

	/*/ HSPI 8-BIT
	uint8_t command_prefix = 0x0000;
    HAL_SPI_Transmit(MKS_SPI_PORT, &command_prefix, sizeof(command_prefix), HAL_MAX_DELAY);
    HAL_SPI_Transmit(MKS_SPI_PORT, (uint8_t *)&Command, sizeof(Command), HAL_MAX_DELAY);
    //*/
}

void MKS_TxDataByte(uint8_t pData) {
	//uint16_t word = 0x8000 | ((uint16_t)pData<<7);
	uint8_t word[2];
	word[0] = 0x80 | (pData>>1);
	word[1] = pData<<7;
	HAL_SPI_Transmit(MKS_SPI_PORT, &word, 2, HAL_MAX_DELAY);
}

void MKS_TxData(uint8_t * pData, uint16_t Size) {
	//* HSPI 16-BIT
    for (uint16_t i=0; i<Size; i++) {
    	MKS_TxDataByte(pData[i]);
    }
    //*/

	/* HSPI 8-BIT
    uint8_t data_prefix = 0x01;
	for (uint16_t i=0; i<Size; i++) {
		HAL_SPI_Transmit(MKS_SPI_PORT, &data_prefix, sizeof(uint8_t), HAL_MAX_DELAY);
		HAL_SPI_Transmit(MKS_SPI_PORT, pData, sizeof(uint8_t), HAL_MAX_DELAY);
	}
	//*/
}
