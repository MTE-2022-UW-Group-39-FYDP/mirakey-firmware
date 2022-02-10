#include "mirakey-serial.h"
#include "main.h"

namespace mirakey-serial {

//todo: enable DMA.
// It's a module that would handle the data transmission in parallel to the actual MCU

// 100 ns is like 10MHz

/* from the hal_spi implementation

frequency of the APBx Peripheral Clock (fPCLK)

DataSize = SPI_DATASIZE_8BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |         |     Polling    | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+

DataSize = SPI_DATASIZE_16BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |         |     Polling    | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/32 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+
       @note: TX processes are HAL_SPI_Transmit(), HAL_SPI_Transmit_IT() and HAL_SPI_Transmit_DMA()
*/

/*
 (+) User must implement HAL_SPI_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).
      (+) Call the function HAL_SPI_Init() to configure the selected device with
          the selected configuration:
        (++) Mode
        (++) Direction
        (++) Data Size
        (++) Clock Polarity and Phase
        (++) NSS Management
        (++) BaudRate Prescaler
        (++) FirstBit
        (++) TIMode
        (++) CRC Calculation
        (++) CRC Polynomial if CRC enabled
      (+) Call the function HAL_SPI_DeInit() to restore the default configuration
          of the selected SPIx peripheral.

*/

void MKS_Init() {
    //todo: I'll need to implement/remove these based on need
    HAL_Init();
    
    MKS_SystemClock_Config();
    MKS_GPIO_Init();
}

void MKS_Deinit() {

static void MKS_GPIO_Init(void) {
    /*    
    todo: look up what the different pin modes do.
    */
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /*MKS_CLK*/
    //todo: the clock has special hardware
    GPIO_InitStruct.Pin = A5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(A5_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_MOSI*/
    GPIO_InitStruct.Pin = A7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(A7_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_SHHLD*/
    //todo: see if you can sync this guy with the clock when you need it.
    GPIO_InitStruct.Pin = B6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(B6_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_RST*/
    GPIO_InitStruct.Pin = B7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(B7_GPIO_Port, &GPIO_InitStruct);
}

void MKS_Select(uint8_t slave_address) {

}



void MKS_Command(uint8_t command) {
    // cheating with HAL
    uint8_t tx[2] = {0x1, command};
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, tx, sizeof(tx), 5000);
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
    
    HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,
                                 hspi->TxXferCount)
}


}
