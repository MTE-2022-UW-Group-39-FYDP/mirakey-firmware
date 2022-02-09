#include "mirakey-serial.h"

namespace mirakey-serial {

void init() {
    // config GPIO
    /*
    PA4 SP1_NSS
    PA5 SP1_SCK
    PA6 SP1_MISO
    PA7 SP1_MOSI
    */
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi) {

}

void select(uint8_t slave_address) {

}

void command(uint8_t command) {

}

void data(uint8_t data) {

}


}