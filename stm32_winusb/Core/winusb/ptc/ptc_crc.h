/*
 * ptc_crc.h
 *
 *  Created on: Feb 11, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_PTC_PTC_CRC_H_
#define WINUSB_PTC_PTC_CRC_H_

#include <stdint.h>
#define PTC_CRC_INIT 0xFFFF

uint16_t ptc_crc_calc(const uint8_t* data, uint16_t len, uint16_t crc_init);

#endif /* WINUSB_PTC_PTC_CRC_H_ */
