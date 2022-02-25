/*
 * buffer.c
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#include <stdio.h>
#include "buffer.h"
#include "buffer_def.h"

static void buffer_block(buffer_typedef_t* buf) {
	if(buf->mutex){
		xSemaphoreTake(buf->mutex, portMAX_DELAY);
	}
}

static void buffer_release(buffer_typedef_t* buf) {
	if(buf->mutex) {
		xSemaphoreGive(buf->mutex);
	}
}

buffer_t buffer_init(buffer_stack_t* buf_stack, const uint8_t* buf_data, uint16_t buf_size, bool safe) {
	//! Validate input param
	if(buf_stack == NULL || buf_data == NULL || buf_size < BUFFER_DATA_SIZE_MINIMUM) {
		return NULL;
	}

	//! Get buffer instance from buffer stack
	buffer_typedef_t* buffer = (buffer_typedef_t*)buf_stack;
	buffer->head = 0;
	buffer->tail = 0;
	buffer->size = buf_size;
	buffer->data = buf_data;

	//! initialize data resource safe call
	if (safe) {
		buffer->mutex = xSemaphoreCreateMutexStatic(&buffer->_mutex_buf);
		if (buffer->mutex == NULL) {
			return NULL;
		}
	} else {
		buffer->mutex = NULL;
	}

	return (buffer_t)buffer;
}

bool buffer_reset(buffer_t buffer) {
	if(buffer == NULL) {
		return false;
	}

	buffer_typedef_t* buf = (buffer_typedef_t*)buffer;

	buffer_block(buf);

	buf->tail = 0;
	buf->head = 0;

	buffer_release(buf);
	return true;
}

bool buffer_available(buffer_t buffer ,uint16_t* len) {
	//! Validate input param
	if(buffer == NULL || len == NULL) {
		return false;
	}

	buffer_typedef_t* buf = (buffer_typedef_t*)buffer;

	buffer_block(buf);
	if(buf->head > buf->tail) {
		*len = buf->size - buf->head + buf->tail;
	} else {
		*len = buf->tail - buf->head;
	}
	buffer_release(buf);

	return true;
}

bool buffer_get(buffer_t buffer ,uint8_t* data, uint16_t data_size, uint16_t* len) {
	//! Validate input param
	if(buffer == NULL || data == NULL || data_size == 0 || len  == NULL) {
		return false;
	}

	buffer_typedef_t* buf = (buffer_typedef_t*)buffer;

	buffer_block(buf);
	*len = 0;
	while(true) {
		//! The data is empty or data len equal to data buffer size(data_size)
		if(buf->tail == buf->head || (*len) >= data_size) {
			break;
		}

		//! Copy data
		data[*len] = buf->data[buf->tail];
		*len += 1;

		//! remove data from buffer
		buf->tail = (buf->tail + 1) % buf->size;
	}

	buffer_release(buf);

	return true;
}

bool buffer_set(buffer_t buffer ,const uint8_t* data, uint16_t len) {
	//! validate input param
	if(buffer == NULL || data == NULL || len == 0) {
		return false;
	}

	buffer_typedef_t* buf = (buffer_typedef_t*)buffer;
	uint16_t i;

	buffer_block(buf);
	for (i = 0; i < len; i++) {
		buf->data[buf->head] = data[i];
		buf->head = (buf->head + 1) % buf->size;

		//! buffer overflow
		if(buf->head == buf->tail) {
			break;
		}
	}
	buffer_release(buf);

	return (i == len);
}
