/*
 * buffer_def.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_BUFFER_DEF_H_
#define WINUSB_BUFFER_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"

typedef struct
{
	uint16_t size;		//! buffer data size
	uint16_t head;		//! buffer write index
	uint16_t tail;		//! buffer read index
	uint8_t* data;		//! Point to buffer data
	SemaphoreHandle_t mutex;		//! Mutex
	StaticSemaphore_t _mutex_buf;	//! Mutex buffer use to create static mutex
} buffer_typedef_t;

#endif /* WINUSB_BUFFER_DEF_H_ */
