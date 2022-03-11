/*
 * key_config.h
 *
 *  Created on: Mar 11, 2022
 *      Author: Will
 */

#ifndef INC_KEY_CONFIG_H_
#define INC_KEY_CONFIG_H_

#define K1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define K2 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define K3 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
#define K4 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)
#define K5 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)
#define K6 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
#define K7 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)
#define K8 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)
#define K9 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)
#define K10 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)
#define K11 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)
#define K12 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)
#define LAYER_KEY HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)	// K12 will be the layer toggle key

#define KEY_DOWN GPIO_PIN_RESET
#define KEY_UP GPIO_PIN_SET

#define NUM_LAYERS 4

// 11 characters per layer, since K12 is the dedicated layer toggle key
#define LAYER_LAYOUTS {{'A','B','C','D','E','F','G','H',' ','I','J'}, \
	{'K','L','M','N','O','P','Q','R',' ','S','T'}, \
	{'U','V','W','X','Y','Z','1','2',' ','3','4'}, \
	{'5','6','7','8','9','0','a','b',' ','c','d'}};

#endif /* INC_KEY_CONFIG_H_ */
