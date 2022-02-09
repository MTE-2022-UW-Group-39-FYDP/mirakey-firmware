#include "mirakey-serial.h"
#include "main.h"

namespace mirakey-serial {


struct mirakey_serial_controller {
    SPI_HandleTypeDef hspi;
    
}

void init() {

    // config GPIO
    /*
    todo: check the pins on the main pcb schematic
    
    PA4 SP1_NSS
    unused
    
    PA5 SP1_SCK
    configure clock edge
    
    PA6 SP1_MISO
    unused
    
    PA7 SP1_MOSI
    pass info through here
    
    [?] SH/HLD
    This has to be a pin I can switch as fast as the clock.
    
    
    MSB vs LSB. I think it'll be MSB
    
    mster mode
    
    serial clo
    
    
    */
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
}

void select(uint8_t slave_address) {

}



void command(uint8_t command) {
    uint8_t tx[2] = {0x1, command};
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, tx, sizeof(tx), 5000);
    // the last param there is the timeout
}

void data(uint8_t data, uint16_t size) {
    uint8_t tx[2] = {0x1, command};
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, tx, sizeof(tx), 5000);
}


}
