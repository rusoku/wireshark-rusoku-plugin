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

#ifndef COMM_BASE_H
#define COMM_BASE_H

#include <stdint.h>
#include "../../main/inc/pcap_param.h"

#define COMM_TOTAL_DEVICES  8
#define MANUFACTURER "RUSOKU"
#define MODEL_TOUCAN "TouCAN"

enum COMM_ERROR_CODES {
    COMM_SUCCESS = 0,
    COMM_LIB_ERROR_NULL = 1,
    COMM_LIB_ERROR = 2,
    COMM_DEVICE_INIT_ERROR = 3,
    COMM_DEVICE_IO_ERROR = 4,
    COMM_POINTER_ERROR = 5
};

enum COMM_MANUFACTURER {
    MANUFACTURER_NONE = 0,
    RUSOKU,
    KVASER,
    PEAK
};

enum COMM_DEVICE_MODEL {
    DEVICE_MODEL_NONE = 0,
    TOUCAN
};

struct COMM_DEVICE {
    uint16_t id;
    enum COMM_MANUFACTURER manufacturer;
    enum COMM_DEVICE_MODEL device_model;
    char manufacturer_str[256];
    char device_model_str[256];
    char serial[256];
};

// CAN flags values
#define CAN_FLAG_STANDARD    0x00000000L
#define CAN_FLAG_EXTENDED    0x00000001L
#define CAN_FLAG_RTR         0x00000002L
#define CAN_FLAG_STATUS      0x00000004L
#define CAN_FLAG_CAN_FD      0x00000008L
#define CAN_FLAG_CAN_XT      0x00000010L
#define CAN_FLAG_SEND        0x80000000L

// CAN error codes
#define CAN_STATUS_OK 0x00    // normal condition.
#define CAN_STATUS_OVERRUN 0x01
#define CAN_STATUS_BUSLIGHT 0x02
#define CAN_STATUS_BUSHEAVY 0x03
#define CAN_STATUS_BUSOFF   0x04

struct COMM_CAN_MSG {
    uint32_t id;
    uint32_t error_status; // error counters (if CAN_FLAG_STATUS is set)
    uint32_t error_code; // error codes
    uint32_t flags; // can flags: STD, EXT, CAN FD etc.
    uint8_t length;
    uint8_t data[64];
};

extern struct COMM_DEVICE comm_devices[];
extern uint32_t comm_device_cnt;

typedef int32_t COMM_DEV_HANDLE;

enum COMM_ERROR_CODES comm_init(char *error_code);

enum COMM_ERROR_CODES comm_deinit(void);

enum COMM_ERROR_CODES comm_get_device_list(struct COMM_DEVICE *comm_devices, uint32_t *num_devices);

enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface);

enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface);

enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle, uint32_t *frame_cnt);

enum COMM_ERROR_CODES comm_read_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg);

enum COMM_ERROR_CODES comm_write_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg);

#endif //COMMUNICATIONS_H
