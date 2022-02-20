/*
 * usb.c
 *
 *  Created on: Feb 10, 2022
 *      Author: phatnt
 */

#include <stdbool.h>

#include "usb.h"
#include "usb_log.h"
#include "usb_assert.h"
#include "buffer.h"
#include "ptc/ptc.h"

#include "usbd_cdc_if.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define USB_TAG  "USB"

static uint8_t host_sync = 0;	//! Host USB sync or not

//! USB send complete notify
volatile bool            send_cplt_flag = false;
static SemaphoreHandle_t send_cplt_notify;			//! USB send complete noitfy
static StaticSemaphore_t _send_cplt_noitfy_buf;

static SemaphoreHandle_t send_mutex;
static StaticSemaphore_t send_mutex_buf;

//! USB buffer
static uint8_t        _tx_buf_data[USB_BUFFER_RX_SIZE];	//! tx data buffer
static buffer_stack_t tx_buf_stack;						//! tx buffer stack
static buffer_t       tx_buf;							//! tx buffer instance

static uint8_t 		  _rx_buf_data[USB_BUFFER_RX_SIZE];	//! rx data buffer
static buffer_stack_t rx_buf_stack;						//! rx buffer stack
static buffer_t       rx_buf;							//! rx buffer instance

extern USBD_HandleTypeDef hUsbDeviceFS;

//! USB tx task
#define USB_TX_TASK_NAME			"usb_tx"	//! USB tx task name
#define USB_TX_TASK_PRI				15			//! USB tx task priority
#define USB_TX_TASK_STACK_SIZE		configMINIMAL_STACK_SIZE
static TaskHandle_t usb_tx_task;
static StaticTask_t usb_tx_task_buf;
static StackType_t usb_tx_task_stack[USB_TX_TASK_STACK_SIZE];
static uint8_t usb_pack[USB_PACKET_SIZE];		//! data packet can send each time

//! USB rx task
#define USB_RX_TASK_NAME			"usb_rx"
#define USB_RX_TASK_PRI				10
#define USB_RX_TASK_STACK_SIZE		configMINIMAL_STACK_SIZE
static TaskHandle_t usb_rx_task;
static StaticTask_t usb_rx_task_buf;
static StackType_t usb_rx_task_stack[USB_RX_TASK_STACK_SIZE];
static bool usb_pending_recv = false;
static uint8_t rx_data_packet_buf[PTC_DATA_SIZE];

static void usb_tx_handle(void* param);
static void usb_rx_handle(void* param);

void usb_init(void)
{
	//! Log initialize
	usb_log_init();
	usb_logi(USB_TAG, "Log initialize");

	//! Send notify semaphore initialize
	send_cplt_notify = xSemaphoreCreateBinaryStatic(&_send_cplt_noitfy_buf);
	usb_assert(send_cplt_notify != NULL);
	usb_logi(USB_TAG, "Initialize send_cplt_notify semaphore -> success");

	send_mutex = xSemaphoreCreateMutexStatic(&send_mutex_buf);
	usb_assert(send_mutex  != NULL);
	usb_logi(USB_TAG, "Initialize send_mutex mutex -> success");

	//! Buffer initialize
	tx_buf = buffer_init(&tx_buf_stack, _tx_buf_data, sizeof(_tx_buf_data), true);
	usb_assert(tx_buf != NULL);
	usb_logi(USB_TAG, "Initialize USB tx_buf -> success");

	rx_buf = buffer_init(&rx_buf_stack, _rx_buf_data, sizeof(_rx_buf_data), false);
	usb_assert(rx_buf != NULL);
	usb_logi(USB_TAG, "Initialize USB rx_buf -> success");

	//! Task initialize
	usb_tx_task = xTaskCreateStatic(usb_tx_handle,
			                USB_TX_TASK_NAME,
							USB_TX_TASK_STACK_SIZE,
							NULL,
							USB_TX_TASK_PRI,
							usb_tx_task_stack,
							&usb_tx_task_buf);
	usb_assert(usb_tx_task != NULL);
	usb_logi(USB_TAG, "Initialize usb_tx_handle task -> success");

	usb_rx_task = xTaskCreateStatic(usb_rx_handle,
				                    USB_RX_TASK_NAME,
									USB_RX_TASK_STACK_SIZE,
									NULL,
									USB_RX_TASK_PRI,
									usb_rx_task_stack,
									&usb_rx_task_buf);
	usb_assert(usb_rx_task != NULL);
	usb_logi(USB_TAG, "Initialize usb_rx_handle task -> success");
}

void usb_send_cplt(void)
{
	BaseType_t higher_priority_task_woken = pdFALSE;

	//! Check that last time data send to USB but timeout
	if(send_cplt_flag == false) {
		return;
	}

	//! Notify USB send data complete.
	send_cplt_flag = false;	//! Clear USB send flag
	xSemaphoreGiveFromISR(send_cplt_notify, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

void usb_suspend(void)
{
	host_sync = USB_HOST_DISCONNECTED;
	usb_logi(USB_TAG, "USB cable removed");
}

uint8_t usb_host_sync(void)
{
	return host_sync;
}

void usb_set_host_sync_active(void)
{
	host_sync = USB_HOST_CONNECTED;
}

uint8_t usb_send(const uint8_t* data, uint16_t len)
{
	uint8_t ret = USB_SEND_SUCCESS;

	//! Validate input param
	if(data == NULL || len ==  0) {
		ret = USB_SEND_PARAM;
		goto send_exit;
	}

	//! Block send
	xSemaphoreTake(send_mutex, portMAX_DELAY);

	//! Make sure host connected
	if (host_sync == USB_HOST_DISCONNECTED) {
		ret = USB_SEND_HOST;
		goto send_exit;
	}

	//! Write data to buffer
	uint16_t data_available;
	if (buffer_available(tx_buf, &data_available) == false) {
		ret = USB_SEND_BUFFER;
		goto send_exit;
	}

	if(len > data_available) {
		ret = USB_SEND_BUFFER;
		goto send_exit;
	}

	if(buffer_set(tx_buf, data, len) == false) {
		ret = USB_SEND_BUFFER;
		goto send_exit;
	}

	//! Notify to task start send data
	xTaskNotifyGive(usb_tx_task);

	send_exit:
	//! Release send
	xSemaphoreGive(send_mutex);

	return ret;
}

void usb_recv(const uint8_t* data, uint16_t len) {
	usb_logi(USB_TAG, "[Recv_raw]: len = %d", len);

	//FIXME For test only
	//! Active host sync if received data
	host_sync = true;

	//! write received data to buffer
	if(buffer_set(rx_buf, data, len) == false) {
		usb_loge(USB_TAG, "rx_buf overflow");
		usb_assert(false);
	}

	//! check buffer available to received next packet
	uint16_t data_len;
	buffer_available(rx_buf, &data_len);
	if(data_len >= USB_PACKET_SIZE) {
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
	} else  {
		usb_logi(USB_TAG, "Pending receive");
		usb_pending_recv = true;
	}

	//! notify data ready.
	BaseType_t higher_priority_task_woken;
	vTaskNotifyGiveFromISR(usb_rx_task, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

static void usb_send_issue(void) {
	host_sync = USB_HOST_DISCONNECTED;
	buffer_reset(tx_buf);
}

static void usb_tx_handle(void* param) {
	(void)param;
	BaseType_t ret;
	uint32_t task_notify;

	for (;;) {
		//! wait for data available on buffer
		task_notify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		usb_logi(USB_TAG, "[Task recv] data ready");

		if (task_notify) {
			//! Send data
			uint16_t data_len;
			do {
				// Get data
				buffer_get(tx_buf, usb_pack, sizeof(usb_pack), &data_len);
				usb_logi(USB_TAG, "[Task recv] Get data: %d", data_len);

				if (data_len > 0) {
					if (CDC_Transmit_FS(usb_pack, data_len) == USBD_OK) {	//! USB transmit success
						send_cplt_flag = true;

						//! Wait send complete
						ret = xSemaphoreTake(send_cplt_notify, pdMS_TO_TICKS(USB_SEND_TIMEOUT));
						if (ret != pdPASS) {
							send_cplt_flag = false;
							usb_loge(USB_TAG, "CDC transmit timeout");

							//! Reset buffer and exit send loop
							usb_send_issue();
						}
					} else {	//! USB transmit failure
						usb_loge(USB_TAG, "CDC transmit failure");

						// Reset buffer and exit send loop
						usb_send_issue();
						data_len = 0;
					}
				}
			} while (data_len);
		}
	}
}

static void usb_rx_handle(void* param) {
	(void)param;

	// Handle data as packet
	uint32_t task_notify;

	for(;;) {
		//! Wait data received ready
		task_notify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		//! Data ready received
		if(task_notify) {
			uint16_t data_len;
			uint16_t data_available;

			while(true) {
				//! Get data
				if (buffer_get(rx_buf, rx_data_packet_buf, sizeof(rx_data_packet_buf), &data_len) == false) {
					usb_loge(USB_TAG, "Task Rx get buffer data failure");
					usb_assert(false);
					break;
				}

				//! Release USB receive pending
				if (usb_pending_recv) {
					buffer_available(rx_buf, &data_available);
					if (data_available >= USB_PACKET_SIZE) {
						usb_pending_recv = false;
						USBD_CDC_ReceivePacket(&hUsbDeviceFS);	//! Ready to receive next packet
					}
				}

				//! Exit loop.
				if(data_len == 0) {
					break;
				}

				//! parse data received as packet
				ptc_recv_packet_t* pack;
				for(uint16_t i = 0; i < data_len; i++) {
					pack = ptc_recv(rx_data_packet_buf[i]);
					if(pack) {
						//TODO Callback to who subscribe USB data.
						usb_logi(USB_TAG, "[Received] CMD: %d, LEN: %d", pack->cmd, pack->len);
					}
				}
			}
		}
	}
}
