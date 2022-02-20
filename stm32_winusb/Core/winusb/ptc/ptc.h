/*
 * ptc.h
 *
 *  Created on: Feb 11, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_PTC_PTC_H_
#define WINUSB_PTC_PTC_H_

#include <stdint.h>
#include <stdbool.h>
#include "ptc_def.h"

typedef struct {
	uint16_t len;
	uint16_t cmd;
	uint8_t *data;
} ptc_recv_packet_t;

typedef struct {
	uint16_t len;
	uint8_t* data;
} ptc_send_packet_t;

/**
 * @brief Reset the protocol packet
 */
void ptc_reset(void);

/**
 * @brief Receive data as packet.
 * @param data the byte data receive
 * @return point to received packet. if null packet do not received.
 */
ptc_recv_packet_t* ptc_recv(uint8_t data);

/**
 * @brief Send data as packet. if parse packet success return the pointer to packet, else failure
 */
ptc_send_packet_t* ptc_send(uint16_t len, uint16_t cmd,  const uint8_t* data);

#endif /* WINUSB_PTC_PTC_H_ */
