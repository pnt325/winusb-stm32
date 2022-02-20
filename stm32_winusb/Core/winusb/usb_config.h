/*
 * usb_config.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef _USB_CONFIG_H_
#define _USB_CONFIG_H_

#include <stdint.h>

//! USB RELEASE
// #define USB_RELEASE

//! USB DEBUG
#ifndef USB_RELEASE
#define USB_DEBUG
#endif

//! USB LOG
#ifdef USB_DEBUG
#define USB_LOG
#endif

//! USB ASSERT
#ifdef USB_LOG
#define USB_ASSERT
#endif

//! USB BUFFER
#define USB_BUFFER_TX_SIZE		2048
#define USB_BUFFER_RX_SIZE		2048


//! USB PACKET
#define USB_PACKET_SIZE			64
#define USB_SEND_TIMEOUT		100	//! ms





#endif /* _USB_CONFIG_H_ */
