/*
 * ptc_def.h
 *
 *  Created on: Feb 11, 2022
 *      Author: phatnt
 */

#ifndef WINUSB_PTC_PTC_DEF_H_
#define WINUSB_PTC_PTC_DEF_H_

#include "ptc_config.h"

/**
 * +--------+--------+--------+-----------+--------+--------+
 * | SOP[1] | LEN[2] | CMD[2] | DATA[256] | CRC[2] | EOP[1] |
 * +--------+--------+--------+-----------+--------+--------+
 *
 * SOP : start of packet
 * LEN : Data length
 * CMD : Data command
 * DATA: Data payload
 * CRC : Data checksum
 * EOP : end of packet
 */

#define PTC_SOP			0x7E
#define PTC_EOP			0X7E
#define PTC_LEN_SIZE	1
#define PTC_SOP_SIZE	1
#define PTC_EOP_SIZE	2
#define PTC_CMD_SIZE	2
#define PTC_CRC_SIZE	2
#define PTC_DATA_SIZE	256
#define PTC_SIZE 		(PTC_SOP_SIZE + PTC_LEN_SIZE + PTC_CMD_SIZE + PTC_DATA_SIZE + PTC_CRC_SIZE + PTC_EOP_SIZE)


enum {
	PTC_STEP_SOP  = 0,	//! step start of packet.
	PTC_STEP_LEN,		//! step data length.
	PTC_STEP_CMD,		//! step data command.
	PTC_STEP_DATA,		//! step data.
	PTC_STEP_CRC,		//! step checksum.
	PTC_STEP_EOP		//! step end of packet.
};

#endif /* WINUSB_PTC_PTC_DEF_H_ */
