//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2025 Gediminas Simanskis, Rusoku technologijos UAB, Vilnius, Lithuania (info@rusoku.com)
//  All rights reserved.
//
//  GNU General Public License v3.0 or later:
//  Rusoku wireshark extcap plugin is free software: you can redistribute it and/or modify
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

#ifndef COMM_RUSOKU_APPLE_H
#define COMM_RUSOKU_APPLE_H

#include "../inc/CANAPI_Types.h"

#define CAN_MAX_HANDLES         (8)     // maximum number of open handles
#define TOUCAN_GET_HARDWARE_VERSION    (CANPROP_GET_VENDOR_PROP + 0x10U)  /**< hardware version as "0xggrrss00" (uint23_t) */
#define TOUCAN_GET_FIRMWARE_VERSION    (CANPROP_GET_VENDOR_PROP + 0x11U)  /**< firmware version as "0xggrrss00" (uint23_t) */
#define TOUCAN_GET_BOOTLOADER_VERSION  (CANPROP_GET_VENDOR_PROP + 0x12U)  /**< boot-loader version as "0xggrrss00" (uint23_t) */
#define TOUCAN_GET_SERIAL_NUMBER       (CANPROP_GET_VENDOR_PROP + 0x13U)  /**< serial no. in hex (uint23_t) */
#define TOUCAN_GET_VID_PID             (CANPROP_GET_VENDOR_PROP + 0x16U)  /**< VID & PID (uint23_t) */
#define TOUCAN_GET_DEVICE_ID           (CANPROP_GET_VENDOR_PROP + 0x17U)  /**< device id. (uint23_t) */
#define TOUCAN_GET_VENDOR_URL          (CANPROP_GET_VENDOR_PROP + 0x18U)  /**< URL of Rusoku's website (uint23_t) */

struct SChannelInfo {
    int32_t m_nChannelNo; ///< channel no. at actual index in the interface list
    char m_szDeviceName[CANPROP_MAX_BUFFER_SIZE]; ///< device name at actual index in the interface list
    char m_szDeviceDllName[CANPROP_MAX_BUFFER_SIZE]; ///< file name of the DLL at actual index in the interface list
    int32_t m_nLibraryId; ///< library id at actual index in the interface list
    char m_szVendorName[CANPROP_MAX_BUFFER_SIZE]; ///< vendor name at actual index in the interface list
    uint32_t m_nSerialNumber;
};

typedef int (*can_test_fp)(int32_t channel, uint8_t mode, const void *param, int *result);

typedef int (*can_init_fp)(int32_t channel, uint8_t mode, const void *param);

typedef int (*can_exit_fp)(int handle);

typedef int (*can_kill_fp)(int handle);

typedef int (*can_start_fp)(int handle, const can_bitrate_t *bitrate);

typedef int (*can_reset_fp)(int handle);

typedef int (*can_write_fp)(int handle, const can_message_t *message, uint16_t timeout);

typedef int (*can_read_fp)(int handle, can_message_t *message, uint16_t timeout);

typedef int (*can_status_fp)(int handle, uint8_t *status);

typedef int (*can_busload_fp)(int handle, uint8_t *load, uint8_t *status);

typedef int (*can_bitrate_fp)(int handle, can_bitrate_t *bitrate, can_speed_t *speed);

typedef int (*can_property_fp)(int handle, uint16_t param, void *value, uint32_t nbyte);

typedef char * (*can_hardware_fp)(int handle);

typedef char * (*can_version_fp)(void);

#endif //COMM_RUSOKU_APPLE_H
