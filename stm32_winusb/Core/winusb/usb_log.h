/*
 * usb_log.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_USB_LOG_H_
#define WINUSB_USB_LOG_H_

#include <stdio.h>
#include "usb_config.h"
#include "SEGGER_RTT.h"

#ifdef USB_LOG
#define usb_log_init()		SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM)
#define usb_log(__s, ...)	printf(__s, ##__VA_ARGS__)
#define usb_loge(__t ,__s, ...)	printf("[ERR] "__t", "__s"\n", ##__VA_ARGS__)
#define usb_logi(__t ,__s, ...)	printf("[INFO] "__t", "__s"\n", ##__VA_ARGS__)
#else
#define usb_log_init()
#define usb_log(__s, ...)
#define usb_loge(__t ,__s, ...)
#define usb_logi(__t ,__s, ...)
#endif

#endif /* WINUSB_USB_LOG_H_ */
