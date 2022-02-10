/* Mirakey Serial Protocol
by Francisco Garccia-Gonzalez
University of Waterloo Undergraduate Mechatronics Engineering - W2022
MTE482 Fourth Year Design Project: Mirakey

*/

namespace mirakey_serial {

/* Protocol Interface */
void MKS_Init();
void MKS_Select(uint8_t SlaveAddress);
void MKS_TxCommand(uint8_t command);
void MKS_TxData(uint8_t * pData, uint16_t Size);

/* Helper Methods */
static void MKS_GPIO_Init(void);
}
