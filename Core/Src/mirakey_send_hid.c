/*
 * mirakey_send_hid.c
 *
 *  Created on: Mar 11, 2022
 *      Author: Will
 */

#include "usbd_hid.h"
#include "usb_hid_keys.h"

#define CONSECUTIVE_SEND_INITIAL_DELAY_MS 500

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
}subKeyBoard;

subKeyBoard keyBoardHIDsub = {0,0,0,0,0,0,0,0};

const char ascii_to_hid_key_map[165][2] = {
	{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,KEY_BACKSPACE},{0,0},{0,0},{0,0},{0,0},{0,KEY_ENTER},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,KEY_ESC},{0,0},{0,0},{0,0},{0,0},
    {0, KEY_SPACE}, {KEY_MOD_LSHIFT, KEY_1}, {KEY_MOD_LSHIFT, KEY_APOSTROPHE},
    {KEY_MOD_LSHIFT, KEY_3}, {KEY_MOD_LSHIFT, KEY_4}, {KEY_MOD_LSHIFT, KEY_5},
    {KEY_MOD_LSHIFT, KEY_7}, {0, KEY_APOSTROPHE}, {KEY_MOD_LSHIFT, KEY_9},
    {KEY_MOD_LSHIFT, KEY_0}, {KEY_MOD_LSHIFT, KEY_8}, {KEY_MOD_LSHIFT, KEY_EQUAL},
    {0, KEY_COMMA}, {0, KEY_MINUS}, {0, KEY_DOT}, {0, KEY_SLASH}, {0, KEY_0},
    {0, KEY_1}, {0, KEY_2}, {0, KEY_3}, {0, KEY_4}, {0, KEY_5}, {0, KEY_6},
    {0, KEY_7}, {0, KEY_8}, {0, KEY_9}, {KEY_MOD_LSHIFT, KEY_SEMICOLON},
    {0, KEY_SEMICOLON}, {KEY_MOD_LSHIFT, KEY_COMMA}, {0, KEY_EQUAL},
    {KEY_MOD_LSHIFT, KEY_DOT}, {KEY_MOD_LSHIFT, KEY_SLASH}, {KEY_MOD_LSHIFT, KEY_2},
    {KEY_MOD_LSHIFT, KEY_A}, {KEY_MOD_LSHIFT, KEY_B}, {KEY_MOD_LSHIFT, KEY_C},
    {KEY_MOD_LSHIFT, KEY_D}, {KEY_MOD_LSHIFT, KEY_E}, {KEY_MOD_LSHIFT, KEY_F},
    {KEY_MOD_LSHIFT, KEY_G}, {KEY_MOD_LSHIFT, KEY_H}, {KEY_MOD_LSHIFT, KEY_I},
    {KEY_MOD_LSHIFT, KEY_J}, {KEY_MOD_LSHIFT, KEY_K}, {KEY_MOD_LSHIFT, KEY_L},
    {KEY_MOD_LSHIFT, KEY_M}, {KEY_MOD_LSHIFT, KEY_N}, {KEY_MOD_LSHIFT, KEY_O},
    {KEY_MOD_LSHIFT, KEY_P}, {KEY_MOD_LSHIFT, KEY_Q}, {KEY_MOD_LSHIFT, KEY_R},
    {KEY_MOD_LSHIFT, KEY_S}, {KEY_MOD_LSHIFT, KEY_T}, {KEY_MOD_LSHIFT, KEY_U},
    {KEY_MOD_LSHIFT, KEY_V}, {KEY_MOD_LSHIFT, KEY_W}, {KEY_MOD_LSHIFT, KEY_X},
    {KEY_MOD_LSHIFT, KEY_Y}, {KEY_MOD_LSHIFT, KEY_Z}, {0, KEY_LEFTBRACE},
    {0, KEY_BACKSLASH}, {0, KEY_RIGHTBRACE}, {KEY_MOD_LSHIFT, KEY_6},
    {KEY_MOD_LSHIFT, KEY_MINUS}, {0, KEY_GRAVE}, {0, KEY_A}, {0, KEY_B},
    {0, KEY_C}, {0, KEY_D}, {0, KEY_E}, {0, KEY_F}, {0, KEY_G}, {0, KEY_H},
    {0, KEY_I}, {0, KEY_J}, {0, KEY_K}, {0, KEY_L}, {0, KEY_M}, {0, KEY_N},
    {0, KEY_O}, {0, KEY_P}, {0, KEY_Q}, {0, KEY_R}, {0, KEY_S}, {0, KEY_T},
    {0, KEY_U}, {0, KEY_V}, {0, KEY_W}, {0, KEY_X}, {0, KEY_Y}, {0, KEY_Z},
    {KEY_MOD_LSHIFT, KEY_LEFTBRACE}, {KEY_MOD_LSHIFT, KEY_BACKSLASH},
    {KEY_MOD_LSHIFT, KEY_RIGHTBRACE}, {KEY_MOD_LSHIFT, KEY_GRAVE},
	{0,0}, //Layer toggle
	{0, KEY_LEFT},{0, KEY_UP},{0, KEY_RIGHT},{0, KEY_DOWN},
	{0, KEY_SYSRQ},{0, KEY_HOME},{0, KEY_PAGEUP},{0, KEY_PAGEDOWN},{0, KEY_END},
	{0, KEY_MEDIA_PLAYPAUSE},{0, KEY_MEDIA_PLAYPAUSE},{0, KEY_MEDIA_PREVIOUSSONG},{0, KEY_MEDIA_NEXTSONG},{0, KEY_MEDIA_VOLUMEUP},{0, KEY_MEDIA_VOLUMEDOWN},
	{KEY_MOD_LCTRL, KEY_S},{KEY_MOD_LCTRL, KEY_P},{KEY_MOD_LCTRL, KEY_F},{KEY_MOD_LCTRL, KEY_Z},{KEY_MOD_LCTRL, KEY_Y},
	{0, KEY_CUT},{0, KEY_COPY},{0, KEY_PASTE},{KEY_MOD_LCTRL, KEY_N},
	{0, KEY_A},{0, KEY_B},{0, KEY_C},{0, KEY_D},{0, KEY_E},{0, KEY_F},{0, KEY_G},{0, KEY_H},{0, KEY_I},{0, KEY_J},{0, KEY_K},{0, KEY_L}
};

uint32_t time_since_last_send[165] = {};
uint8_t most_recent_char_sent = 0x00;
uint32_t num_times_recent_char_sent = 0;

void hid_send_char(uint8_t c)
{
	// TODO: Implement 6 key rollover using a circular queue for the HID report keycode slots

	if (most_recent_char_sent == c && num_times_recent_char_sent == 1) {
		if (HAL_GetTick() - time_since_last_send[c] < CONSECUTIVE_SEND_INITIAL_DELAY_MS) {
			return;
		}
	}

    keyBoardHIDsub.MODIFIER = ascii_to_hid_key_map[c][0];
    keyBoardHIDsub.KEYCODE1 = ascii_to_hid_key_map[c][1];
    USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
	HAL_Delay(25);

	keyBoardHIDsub.MODIFIER=0x00;
	keyBoardHIDsub.KEYCODE1=0x00;
	USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
	HAL_Delay(25);

	time_since_last_send[c] = HAL_GetTick();
	if (most_recent_char_sent != c) {
		most_recent_char_sent = c;
		num_times_recent_char_sent = 1;
	} else {
		num_times_recent_char_sent++;
	}
}
