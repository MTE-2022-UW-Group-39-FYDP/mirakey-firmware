/*
 * mirakey_send_hid.h
 *
 *  Created on: Mar 11, 2022
 *      Author: Will
 */

#ifndef INC_MIRAKEY_SEND_HID_H_
#define INC_MIRAKEY_SEND_HID_H_

uint32_t num_times_recent_char_sent;

void hid_send_char(char c);

#endif /* INC_MIRAKEY_SEND_HID_H_ */
