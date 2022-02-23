#include "mirakey-serial.h"

void MKS_ActivateDisplay(uint8_t SlaveAddress) {
    MKS_Select(SlaveAddress);    
    MKS_TxCommand(0x20); //Set Memory Addressing Mode
    MKS_TxCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                         // 10b,Page Addressing Mode (RESET); 11b,Invalid
    MKS_TxCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
}

void MKS_Init() {
    // powers up the displays
    // activate reset line

    for(all displays) {
        ActivateDisplay(displayAddress);
    }
}


void SendGlyph(uint8_t SlaveAddress, char* GlyphBuffer) {
    MKS_Select(SlaveAddress);

}

void MKS_Select(uint8_t slave_address) {
    // todo: sync the register clock with SPI clock
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, slave_address, sizeof(tx), 5000);
    // todo: desync register clock

}

void MKS_Command(uint8_t command) {
    // cheating with HAL
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, 0x01, sizeof(tx), 5000);
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, command, sizeof(tx), 5000);
}

    
/*
MSB vs LSB. I think it'll be MSB
mster mode
serial clo
*/
void MKS_Data(uint8_t data, uint16_t size) {

    // let's try something more intelligent
      
    /* Set the transaction information */
    hspi->State       = HAL_SPI_STATE_BUSY_TX;
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
    hspi->pTxBuffPtr  = (uint8_t *)pData;
    hspi->TxXferSize  = Size;
    hspi->TxXferCount = Size;

    /* Init field not used in handle to zero */
    hspi->pRxBuffPtr  = (uint8_t *)NULL;
    hspi->TxISR       = NULL;
    hspi->RxISR       = NULL;
    hspi->RxXferSize  = 0U;
    hspi->RxXferCount = 0U;
    
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    //__HAL_SPI_DISABLE(hspi);
    //SPI_1LINE_TX(hspi);
    
    HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR, hspi->TxXferCount)
}
