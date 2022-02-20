/*
 * winusb.c
 *
 *  Created on: Feb 9, 2022
 *      Author: phatnt
 */

#include <stdio.h>

#include "winusb.h"
#include "winusb_def.h"
#include "usbd_ioreq.h"

uint8_t winusb_get_vendor_code(void)
{
	return WINUSB_VENDOR_CODE;
}

uint8_t* winusb_get_ms_os_string_desc(uint16_t* len)
{
	printf("[GET] WINUSB_MS_OS_STRING_DESC\n");
	*len = WINUSB_MS_OS_STRING_DESC_LEN;
	return (uint8_t*)WINUSB_MS_OS_STRING_DESC;
}

static uint8_t* winusb_get_compat_id_os_desc(uint16_t* len)
{
	printf("[GET] WINUSB_COMPAT_ID_OS_DESC\n");
	*len = WINUSB_COMPAT_ID_OS_DESC_LEN;
	return (uint8_t*) WINUSB_COMPAT_ID_OS_DESC;
}

static uint8_t* winusb_get_prop_os_desc(uint16_t* len)
{
	printf("[GET] WINUSB_PROP_OS_DESC\n");
	*len = WINUSB_PROP_OS_DESC_LEN;
	return (uint8_t*) WINUSB_PROP_OS_DESC;
}

void winusb_get_desc(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {
	uint16_t len;
	uint8_t *pbuf;

	switch (req->wIndex) {
	case 0x04:
		pbuf = winusb_get_compat_id_os_desc(&len);
		break;
	case 0x05:
		pbuf = winusb_get_prop_os_desc(&len);
		break;
	default:
		USBD_CtlError(pdev, req);
		break;
	}

	if (len != 0 && req->wLength != 0) {
		len = MIN(len, req->wLength);
		USBD_CtlSendData(pdev, pbuf, len);
	}
}
