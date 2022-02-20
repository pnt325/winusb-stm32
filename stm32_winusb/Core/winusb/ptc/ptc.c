/*
 * ptc.c
 *
 *  Created on: Feb 11, 2022
 *      Author: phatnt
 */

#include <stdio.h>
#include "ptc.h"
#include "ptc_def.h"
#include "ptc_crc.h"

static uint8_t send_buf[PTC_SIZE];
static uint8_t recv_buf[PTC_DATA_SIZE];

static uint8_t step = PTC_STEP_SOP;
static uint16_t index;
static uint8_t* tmp_buf;
static uint16_t crc;

static ptc_send_packet_t send_pack;
static ptc_recv_packet_t recv_pack;

void ptc_reset(void) {
	step = 0;
	recv_pack.data = recv_buf;
	send_pack.data = send_buf;
}

ptc_recv_packet_t* ptc_recv(uint8_t data) {
	ptc_recv_packet_t* ret = NULL;

	switch (step) {
	case PTC_STEP_SOP:
		if (data == PTC_SOP) {
			step = PTC_STEP_LEN;
			tmp_buf = (uint8_t*) &recv_pack.len;
			index = 0;
		}
		break;
	case PTC_STEP_LEN:
		tmp_buf[index++] = data;
		if (index >= PTC_LEN_SIZE) {
			step = PTC_STEP_CMD;
			tmp_buf = (uint8_t*) &recv_pack.cmd;
			index = 0;
		}
		break;
	case PTC_STEP_CMD:
		tmp_buf[index++] = data;
		if(index >= PTC_CMD_SIZE) {
			step = PTC_STEP_DATA;
			index = 0;
		}
		break;
	case PTC_STEP_DATA:
		recv_pack.data[index++] = data;
		if (index >= recv_pack.len) {
			step = PTC_STEP_CRC;
			index = 0;
			tmp_buf = (uint8_t*)&crc;
		}
		break;
	case PTC_STEP_CRC:
		tmp_buf[index++] = data;
		if(index >= PTC_CRC_SIZE) {
			step = PTC_STEP_EOP;
		}
		break;
	case PTC_STEP_EOP:
		step = PTC_STEP_SOP;
		if(data == PTC_EOP) {
			//! checksum packet received.
			uint16_t crc_calc = PTC_CRC_INIT;
			crc_calc = ptc_crc_calc((const uint8_t*) &recv_pack.len, PTC_CRC_SIZE, crc_calc);
			crc_calc = ptc_crc_calc((const uint8_t*) &recv_pack.cmd, PTC_CMD_SIZE, crc_calc);
			crc_calc = ptc_crc_calc((const uint8_t*) recv_pack.data, recv_pack.len, crc_calc);
			if(crc_calc == crc) {
				ret = &recv_pack;
			}
		}
		break;
	default:
		break;
	}

	return ret;
}

ptc_send_packet_t* ptc_send(uint16_t len, uint16_t cmd, const uint8_t* data) {
	uint8_t* tmp;
	uint16_t index = 0;

	if(len > PTC_LEN_SIZE || data == NULL) {
		return NULL;
	}

	//! SOP
	send_buf[index++] = PTC_SOP;

	//! LEN
	tmp = (uint8_t*)&len;
	send_buf[index++] = tmp[0];
	send_buf[index++] = tmp[1];

	//! CMD
	tmp = (uint8_t*)&cmd;
	send_buf[index++] = tmp[0];
	send_buf[index++] = tmp[1];

	//! DATA
	for(uint16_t i = 0; i < len; i++) {
		send_buf[index++] = data[i];
	}

	//! CRC
	uint16_t crc_val = ptc_crc_calc(&send_buf[1], index - 1, PTC_CRC_INIT);
	tmp_buf = (uint8_t*)&crc_val;
	send_buf[index++] = tmp_buf[0];
	send_buf[index++] = tmp_buf[1];

	//! EOP
	send_buf[index++] = PTC_EOP;

	send_pack.len = index;
	send_pack.data = send_buf;

	return &send_pack;
}


