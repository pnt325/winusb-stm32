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

uint8_t winusb_get_vendor_code(void);
uint8_t* winusb_get_ms_os_string_desc(uint16_t* len);
//uint8_t* winusb_get_compat_id_os_desc(uint16_t* len);
//uint8_t* winusb_get_prop_os_desc(uint16_t* len);
void	 winusb_get_desc(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req);

#endif /* _WINUSB_H_ */
