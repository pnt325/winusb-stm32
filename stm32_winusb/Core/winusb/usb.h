/*
 * usb.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>

/**
 * @brief USB subscribe callback function.
 */
typedef void (*usb_callback_t)(uint16_t cmd, const uint8_t* data, uint16_t len);

enum
{
	USB_SEND_SUCCESS = 0,	//! Send message success
	USB_SEND_TIMEOUT,		//! Send message failure - timeout
	USB_SEND_FAILURE,		//! Send message failure - USB hardware failure
	USB_SEND_PARAM,			//! Send message failure - Input parameter invalid
	USB_SEND_HOST,			//! Send message failure - Host disconnected
	USB_SEND_BUFFER, 		//! Send message failure - Buffer overflow
};

enum
{
	USB_HOST_DISCONNECTED = 0,	//! Host USB service disconnected
	USB_HOST_CONNECTED = 1,		//! Host USB service connected
};

/**
 * @brief Initialize USB
 */
void usb_init(void);

/**
 * @brief USB send data complete callback
 */
void usb_send_cplt(void);

/**
 * @brief USB suspend callback
 */
void usb_suspend(void);

/**
 * @brief Send data to USB
 * @param data point to array where store send data
 * @param len number of byte data to send
 * @return	USB_SEND_SUCCESS : success
 *   		USB_SEND_TIMEOUT : failure
 *   		USB_SEND_FAILURE : failure
 *   		USB_SEND_PARAM   : failure
 *   		USB_SEND_HOST    : failure
 *   		USB_SEND_BUFFER  : failure
 */
uint8_t usb_send(const uint8_t* data, uint16_t len);

void usb_subscribe(void);

/**
 * @brief Handle USB received data
 * @param data point to where data received store
 * @param len number of byte data received
 * @return None
 */
void usb_recv(const uint8_t *data, uint16_t len);

/**
 * @brief Get host sync status
 * @return Host sync status
 * 			USB_HOST_CONNECTED    : Host connected
 * 			USB_HOST_DISCONNECTED : Host disconnected
 */
uint8_t usb_host_sync(void);

/**
 * @brief Set host sync status
 * @return None
 */
void usb_set_host_sync_active(void);

#endif /* _USB_H_ */
