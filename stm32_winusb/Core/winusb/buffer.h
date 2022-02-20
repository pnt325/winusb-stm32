/*
 * buffer.h
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_BUFFER_H_
#define WINUSB_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include "buffer_def.h"

#define BUFFER_DATA_SIZE_MINIMUM		(64*4)		//! Data size in byte

typedef void* buffer_t;
typedef struct
{
	uint8_t _datas[sizeof(buffer_typedef_t)];
}buffer_stack_t;

/**
 * @brief Initialize the buffer
 * @param buf_stack point to buffer_stack, the buffer stack should be static or global
 * @param buf_data point to array of byte where store buffer data, the data should be static or global
 * @param buf_size buffer data size
 * @param safe initialize safe resources safe usage
 * @return buffer instance, NULL if failure
 */
buffer_t buffer_init(buffer_stack_t* buf_stack, const uint8_t* buf_data, uint16_t buf_size, bool safe);

/**
 * @brief Reset the data in buffer to empty
 * @param buffer Buffer instance
 * @return true if success, else otherwise
 */
bool buffer_reset(buffer_t buffer);

/**
 * @brief Get the number of byte data in buffer
 * @param buffer Buffer instance
 * @param [out]len Number of byte
 * @return true if success, else otherwise
 */
bool buffer_available(buffer_t buffer ,uint16_t* len);

/**
 * @brief Read data from buffer
 * @param buffer Buffer instance
 * @param data pointer data where data read
 * @param data_size size of data where store read data
 * @param [out]len Number of byte data read
 * @return true if success, else otherwise
 */
bool buffer_get(buffer_t buffer ,uint8_t* data, uint16_t data_size, uint16_t* len);

/**
 * @brief Write data to buffer
 * @param buffer Buffer insatance
 * @param data point to data
 * @param len data len
 * @return true if success, else otherwise
 */
bool buffer_set(buffer_t buffer ,const uint8_t* data, uint16_t len);

#endif /* WINUSB_BUFFER_H_ */
