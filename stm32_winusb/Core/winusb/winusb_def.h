/*
 * winusb_def.h
 *
 *  Created on: Feb 9, 2022
 *      Author: phatnt
 */

#ifndef _WINUSB_DEF_H_
#define _WINUSB_DEF_H_

#include <stdint.h>
#include "usbd_def.h"

#define WINUSB_VENDOR_CODE		0x21	//! (!)

//////////////////////////////////////////////////////
//         WINUSB_MS_OS_STRING_DESPCRIPTOR			//
//////////////////////////////////////////////////////
#define WINUSB_MS_OS_STRING_DESC_LEN			0x12
const uint8_t WINUSB_MS_OS_STRING_DESC[] = {
	WINUSB_MS_OS_STRING_DESC_LEN,           	//! Length of the descriptor
	0x03,                  						//! Descriptor type - string
	'M', 0x00, 'S', 0x00, 'F', 0x00, 'T', 0x00,	//! MSFT100 signature field
	'1', 0x00, '0', 0x00, '0', 0x00,
	WINUSB_VENDOR_CODE,         				//! Vendor-specific vendor code
	0x00                            			//! Pad field
};

#define WINUSB_COMPAT_ID_OS_DESC_LEN				0x28
const uint8_t WINUSB_COMPAT_ID_OS_DESC[] =
{
	0x28, 0x00, 0x00, 0x00, 						//! The length, in bytes, of the complete extended compat ID descriptor
	0x00, 0x01,          							//! The descriptor's version number, in binary coded decimal (BCD) format
	0x04, 0x0,          							//! An index that identifies the particular OS feature descriptor
	0x01,             								//! The number of custom property sections
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 		//! Reserved
	0x00,											//! The interface or function number
	0x00,             								//! Reserved
	'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00, 		//! The function's compatible ID ("WINUSB")
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//! the function's sub-compatible ID (None)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00				//! Reserved
};

#define WINUSB_PROP_OS_DESC_LEN					0x8E
const uint8_t WINUSB_PROP_OS_DESC[] =
{
	0x8E, 0x00, 0x00, 0x00,						//! The length, in bytes, of the complete extended properties descriptor
	0x00, 0x01,   								//! The descriptor's version number, in binary coded decimal (BCD) format
	0x05, 0x00,   								//! The index for extended properties OS descriptor
	0x01, 0x00,   								//! Number of custom property section that follow the header sections
	0x84, 0x00, 0x00, 0x00,						//! The size of this custom properties section
	0x01, 0x00, 0x00, 0x00,						//! Property data format
	0x28, 0x00,        							//! Property name length (PNL)
	'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00,	//! The property name (DeviceInterfaceGUID)
	'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00,
	't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,
	'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00,
	'U', 0x00, 'I', 0x00, 'D', 0x00,
	0x00, 0x00,
	0x4E, 0x00, 0x00, 0x00,  					//! Length of the buffer holding the property data (PDL)
	'{', 0x00, '1', 0x00, '3', 0x00, 'E', 0x00,	//! PDL format-dependent property data
	'B', 0x00, '3', 0x00, '6', 0x00, '0', 0x00,
	'B', 0x00, '-', 0x00, 'B', 0x00, 'C', 0x00,
	'1', 0x00, 'E', 0x00, '-', 0x00, '4', 0x00,
	'6', 0x00, 'C', 0x00, 'B', 0x00, '-', 0x00,
	'A', 0x00, 'C', 0x00, '8', 0x00, 'B', 0x00,
	'-', 0x00, 'E', 0x00, 'F', 0x00, '3', 0x00,
	'D', 0x00, 'A', 0x00, '4', 0x00, '7', 0x00,
	'B', 0x00, '4', 0x00, '0', 0x00, '6', 0x00,
	'2', 0x00, '}', 0x00, 0x00, 0x00,
};


#if 0	// USB FS DEVICE DESCRIPTOR
#define USB_FS_DeviceDescLen			0x12
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_FS_DeviceDescLen] __ALIGN_END = {
	USB_FS_DeviceDescLen, /*bLength */
	USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
#if (USBD_LPM_ENABLED == 1)
	0x01, 0x02, /*bcdUSB */
	/* changed to USB version 2.01 in order to support LPM L1 suspend resume test of USBCV3.0*/
#else
	0x00, 0x02,                      /*bcdUSB 2.1*/
#endif /* (USBD_LPM_ENABLED == 1) */
	0xEF, /*bDeviceClass*/
	0x02, /*bDeviceSubClass*/
	0x01, /*bDeviceProtocol*/
	USB_MAX_EP0_SIZE, /*bMaxPacketSize*/
	LOBYTE(USBD_VID), /*idVendor*/
	HIBYTE(USBD_VID), /*idVendor*/
	LOBYTE(USBD_PID_FS), /*idProduct*/
	HIBYTE(USBD_PID_FS), /*idProduct*/
	0x00, /*bcdDevice rel. 2.00*/
	0x02,
	USBD_IDX_MFC_STR, /*Index of manufacturer  string*/
	USBD_IDX_PRODUCT_STR, /*Index of product string*/
	USBD_IDX_SERIAL_STR, /*Index of serial number string*/
	USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};
#endif

#if 0	// USD FS CDC CONFIGURE DESCRIPTOR
#define WINUSB_CONFIG_DESC_SIZE 32
__ALIGN_BEGIN uint8_t USBD_CDC_CfgFSDesc[WINUSB_CONFIG_DESC_SIZE] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   							/* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  WINUSB_CONFIG_DESC_SIZE,          /* wTotalLength:no of returned bytes */
  0x00,
  0x01,   							/* bNumInterfaces: 1 interface for Game IO */
  0x01,   							/* bConfigurationValue: Configuration value */
  USBD_IDX_CONFIG_STR,   			/* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   							/* bmAttributes: self powered */
  0x32,   							/* MaxPower 0 mA */

  /*---------------------------------------------------------------------------*/

  /* interface descriptor*/
  0x09,   					/* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface, zero based index of this interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: Two endpoints used */
  0xEF,   /* bInterfaceClass: vendor, setting: None */
  0x02,   /* bInterfaceSubClass: ACM*/
  0x01,   /* bInterfaceProtocol: protocol AT*/
  USBD_IDX_INTERFACE_STR,   /* iInterface: index of interface name */

  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      			/* bDescriptorType: Endpoint */
  CDC_OUT_EP,                        	/* bEndpointAddress */
  0x03,                              	/* bmAttributes: interrupt transfer, bulk = 2*/
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x01,                              	/* bInterval: ignore for Bulk transfer */

  /*Endpoint IN Descriptor*/
  0x07,   								/* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      			/* bDescriptorType: Endpoint */
  CDC_IN_EP,                         	/* bEndpointAddress */
  0x03,                              	/* bmAttributes: interrupt transfer, bulk = 2 */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x01                               	/* bInterval: ignore for Bulk transfer */
} ;
#endif

#endif /* _WINUSB_DEF_H_ */
