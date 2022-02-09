// mirakey serial protocol by Francisco Garccia-Gonzalez
// #include spi

namespace mirakey_serial {

void select(uint8_t slave_address);
void command(uint8_t command);
void data(uint8_t data);

}
