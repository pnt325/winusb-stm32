/*
 * winusb.h
 *
 *  Created on: Feb 9, 2022
 *      Author: phatnt
 */

#ifndef _WINUSB_H_
#define _WINUSB_H_

#include <stdint.h>
#include "usbd_def.h"

/**
 * @brief Get USB vendor code
 * @return vendor code value
 */
uint8_t  winusb_get_vendor_code(void);

/**
 * @brief Get Microsoft OS string descriptor
 * @param [out] len Point to the length
 * @return Point to os string descriptor data
 */
uint8_t* winusb_get_ms_os_string_desc(uint16_t* len);

/**
 * @brief Get WINUSB descriptor
 * @param pdev USB handle instance
 * @param req USB request data
 * @return None
 */
void	 winusb_get_desc(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req);

#endif /* _WINUSB_H_ */
