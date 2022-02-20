/*
 * usb_assert.c
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#include "stm32f7xx_hal.h"
#include "usb_assert.h"
#include "usb_log.h"

#ifdef USB_LOG
void __usb_assert(const char* file, int line)
{
	usb_loge("ASSERT", "%s, %d", file, line);
	__disable_irq();
	while(1)
	{
	}
}
#endif
