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

#define NUM_LAYERS 6

// glyph lib indeces
#define ENTER 		13
// arrows layer
#define ESC 		27
#define PRINTSCREEN 133
#define HOME 		134
#define PGUP 		135
#define UP 			130
#define PGDOWN		136
#define END			137
#define LEFT		129
#define DOWN		132
#define RIGHT		131
// media layer
#define PREV		140
#define PLAY		138
#define PAUSE		139
#define NEXT		141
#define VOLUP		142
#define VOLDOWN		143
// shortcut layer
#define CUT			149
#define COPY		150
#define PASTE		151
#define PRINT		145
#define UNDO		147
#define REDO		148
#define SAVE		144
#define FIND		146
#define NEW			152
#define SPACE		32
#define BACKSPACE	8
//

// 11 characters per layer, since K12 is the dedicated layer toggle key
#define LAYER_LAYOUTS {{'A','B','C','D','E','F','G','H','I','J','K'}, \
	{'7','8','9','0','4','5','6',ENTER,'1','2','3'}, \
	{ESC,' ',PRINTSCREEN,HOME,PGUP,UP,PGDOWN,END,LEFT,DOWN,RIGHT}, \
	{PREV,PLAY,PAUSE,NEXT,VOLUP,' ',' ',' ',VOLDOWN,' ',' '}, \
	{CUT,COPY,PASTE,PRINT,UNDO,REDO,SAVE,FIND,NEW,SPACE,BACKSPACE}, \
	{153,154,155,156,157,158,159,160,161,162,163}};

#endif /* INC_KEY_CONFIG_H_ */
