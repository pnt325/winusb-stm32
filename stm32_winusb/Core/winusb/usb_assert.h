/*
 * usb_assert.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_USB_ASSERT_H_
#define WINUSB_USB_ASSERT_H_

#include <stdio.h>
#include "usb_log.h"

#ifdef USB_ASSERT
extern void __usb_assert(const char* file, int line);
#define usb_assert(__v)			(__v)?(void)0:__usb_assert(__FILE__, __LINE__)
#else
#define usb_assert(__v)
#endif

#endif /* WINUSB_USB_ASSERT_H_ */
