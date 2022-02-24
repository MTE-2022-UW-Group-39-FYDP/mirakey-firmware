#include "mirakey-serial.h"

/*-------------------------------------------------------------------------------------------------
Bus Logic
-------------------------------------------------------------------------------------------------*/

void MKS_Init() {
    // powers up the displays
    HAL_GPIO_WritePin(SSD1306_Reset_Port, SSD1306_Reset_Pin, GPIO_PIN_RESET);
    //delay
    for(uint8_t slaveAddress = 0; i<MKS_NUM_KEYS; ) {
        ActivateDisplay(displayAddress);
  }
}

void MKS_ActivateDisplay(uint8_t SlaveAddress) {

    MKS_Select(SlaveAddress);  

    MKS_TxCommand(0xAE); // turn it off
  
    MKS_TxCommand(0x20); //Set Memory Addressing Mode
    MKS_TxCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                         // 10b,Page Addressing Mode (RESET); 11b,Invalid
    MKS_TxCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
    MKS_TxCommand(0xC8); // Set COM output scan direction

    MKS_TxCommand(0x00); //---set low column address
    MKS_TxCommand(0x10); //---set high column address
    MKS_TxCommand(0x40); //--set start line address - CHECK
    MKS_TxCommand(0x81); // set max contrast
    MKS_TxCommand(0xFF); // set max contrast

    MKS_TxCommand(0xA1); // set segment re-map 0 to 127 - CHECK
    MKS_TxCommand(0xA6); // set normal color

    MKS_TxCommand(0xA8); // set multiplex ratio
    MKS_TxCommand(0x3F); // set height

    MKS_TxCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    MKS_TxCommand(0xD3); //-set display offset - CHECK
    MKS_TxCommand(0x00); //-not offset
    MKS_TxCommand(0xD5); //--set display clock divide ratio/oscillator frequency
    MKS_TxCommand(0xF0); //--set divide ratio
    MKS_TxCommand(0xD9); //--set pre-charge period
    MKS_TxCommand(0x22); //
    MKS_TxCommand(0xDA); //--set com pins hardware configuration - CHECK
    
    MKS_TxCommand(0x12); // related to HEIGHT

    #if (SSD1306_HEIGHT  == 72)
    #endif
}

void SendGlyph(uint8_t SlaveAddress, uint8_t* GlyphBuffer) {
    MKS_Select(SlaveAddress);
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        MKS_TxCommand(0xB0 + i); // Set the current RAM page address.
        MKS_TxCommand(0x00);
        MKS_TxCommand(0x10);
        MKS_TxData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

/*-------------------------------------------------------------------------------------------------
Serial Functions
-------------------------------------------------------------------------------------------------*/

void MKS_Select(uint8_t slave_address) {
    // todo: sync the register clock with SPI clock
    HAL_SPI_Transmit(&SSD1306_SPI_PORT, slave_address, sizeof(tx), 5000);
    // todo: desync register clock

}

void MKS_TxCommand(uint8_t command) {
    // cheating with HAL
    HAL_SPI_Transmit(&SSD1306_SPI_PORT, 0x01, sizeof(command), 5000);
    HAL_SPI_Transmit(&SSD1306_SPI_PORT, (uint8_t *)&command, sizeof(command), 5000);
}

void MKS_TxData(uint8_t * dataP, uint16_t Size) {
    for (uint16_t i=0; i<Size; i++) {
        MKS_TxDataByte(dataP[i]);
    }
}

void MKS_TxDataByte(uint8_t data) {
    HAL_SPI_Transmit(&SSD1306_SPI_PORT, 0x00, sizeof(data), 5000);
    HAL_SPI_Transmit(&SSD1306_SPI_PORT, (uint8_t *)&data, sizeof(data), 5000);
}
