//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2025 Gediminas Simanskis, Rusoku technologijos UAB, Vilnius, Lithuania (info@rusoku.com)
//  All rights reserved.
//
//  GNU General Public License v3.0 or later:
//  Rusoku wireshark CAN bus adapters extcap plugin is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Rusoku wireshark extcap plugin is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Rusoku wireshark extcap plugin.If not, see <https://www.gnu.org/licenses/>.

#ifndef COMM_RUSOKU_WIN_H
#define COMM_RUSOKU_WIN_H

#define CANAL_DEVLIST_SIZE_MAX 64

/// Canal error Codes
#define CANAL_ERROR_SUCCESS						0		// All is OK
#define CANAL_ERROR_BAUDRATE					1		// Baudrate error
#define CANAL_ERROR_BUS_OFF						2		// Bus off error
#define CANAL_ERROR_BUS_PASSIVE				    3		// Bus Passive error
#define CANAL_ERROR_BUS_WARNING					4		// Bus warning error
#define CANAL_ERROR_CAN_ID						5		// Invalid CAN ID
#define CANAL_ERROR_CAN_MESSAGE					6		// Invalid CAN message
#define CANAL_ERROR_CHANNEL						7		// Invalid channel
#define CANAL_ERROR_FIFO_EMPTY					8		// FIFO is empty
#define CANAL_ERROR_FIFO_FULL					9		// FIFI is full
#define CANAL_ERROR_FIFO_SIZE					10		// FIFO size error
#define CANAL_ERROR_FIFO_WAIT					11
#define CANAL_ERROR_GENERIC						12		// Generic error
#define CANAL_ERROR_HARDWARE					13		// Hardware error
#define CANAL_ERROR_INIT_FAIL					14		// Initialization failed
#define CANAL_ERROR_INIT_MISSING				15
#define CANAL_ERROR_INIT_READY					16
#define CANAL_ERROR_NOT_SUPPORTED				17		// Not supported
#define CANAL_ERROR_OVERRUN						18		// Overrun
#define CANAL_ERROR_RCV_EMPTY					19		// Receive buffer empty
#define CANAL_ERROR_REGISTER					20		// Register value error
#define CANAL_ERROR_TRM_FULL					21
#define CANAL_ERROR_ERRFRM_STUFF				22		// Errorframe: stuff error detected
#define CANAL_ERROR_ERRFRM_FORM					23		// Errorframe: form error detected
#define CANAL_ERROR_ERRFRM_ACK					24		// Errorframe: acknowledge error
#define CANAL_ERROR_ERRFRM_BIT1					25		// Errorframe: bit 1 error
#define CANAL_ERROR_ERRFRM_BIT0					26		// Errorframe: bit 0 error
#define CANAL_ERROR_ERRFRM_CRC					27		// Errorframe: CRC error
#define CANAL_ERROR_LIBRARY						28		// Unable to load library
#define CANAL_ERROR_PROCADDRESS					29		// Unable get library proc address
#define CANAL_ERROR_ONLY_ONE_INSTANCE			30		// Only one instance allowed
#define CANAL_ERROR_SUB_DRIVER					31		// Problem with sub driver call
#define CANAL_ERROR_TIMEOUT						32		// Blocking call timeout
#define CANAL_ERROR_NOT_OPEN					33 		// The device is not open.
#define CANAL_ERROR_PARAMETER					34 		// A parameter is invalid.
#define CANAL_ERROR_MEMORY						35 		// Memory exhausted.
#define CANAL_ERROR_INTERNAL					36 		// Some kind of internal program error
#define CANAL_ERROR_COMMUNICATION				37 		// Some kind of communication error
#define CANAL_ERROR_USER						38      // Login error

/// ID flags
#define CANAL_IDFLAG_STANDARD       0x00000000	// Standard message id (11-bit)
#define CANAL_IDFLAG_EXTENDED       0x00000001	// Extended message id (29-bit)
#define CANAL_IDFLAG_RTR            0x00000002	// RTR-Frame
#define CANAL_IDFLAG_STATUS         0x00000004	// This package is a status indication (id holds error code)
#define CANAL_IDFLAG_SEND           0x80000000  // Reserved for use by application software to indicate send

/// canal open options
#define CANAL_SILENT_MODE           0x00000001

struct CANAL_STATUS {
    unsigned long channel_status; // Current state for channel
    unsigned long lasterrorcode; // Last error code
    unsigned long lasterrorsubcode; // Last error subcode
    char lasterrorstr[80]; // Last error string
};

struct CANAL_MSG {
    uint32_t flags; // CAN message flags
    uint32_t obid; // Used by driver for channel info etc.
    uint32_t id; // CAN id (11-bit or 29-bit)
    uint8_t sizeData; // Data size 0-8
    uint8_t data[8]; // CAN Data
    uint32_t timestamp; // Relative time stamp for package in microseconds
};

struct CANAL_DEV_INFO {
    uint32_t DeviceId;
    uint32_t vid;
    uint32_t pid;
    char SerialNumber[10];
};

struct CANAL_DEV_LIST {
    struct CANAL_DEV_INFO canDevInfo[CANAL_DEVLIST_SIZE_MAX];
    uint32_t canDevCount;
};

typedef int32_t (*CanalOpen_fp)(const char *pConfigStr, unsigned long flags);

typedef void (*CanalClose_fp)(uint32_t handle);

typedef int32_t (*CanalGetDeviceList_fp)(struct CANAL_DEV_LIST *canal_dev_list, int canal_dev_list_size);

typedef int32_t (*CanalGetSerialNumber_fp)(uint32_t handle, char *pSerialNumberStr);

typedef int32_t (*CanalDataAvailable_fp)(uint32_t handle);

typedef int32_t (*CanalReceive_fp)(uint32_t handle, struct CANAL_MSG *pCanMsg);

typedef int32_t (*CanalSend_fp)(uint32_t handle, struct CANAL_MSG *pCanMsg);

typedef int32_t (*CanalGetStatus_fp)(uint32_t handle, struct CANAL_STATUS *pCanStatus);

#endif //COMM_RUSOKU_WIN_H
