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

#include <dlfcn.h>
#include <string.h>
#include "../inc/comm_rusoku_apple.h"
#include "../../main/inc/comm_base.h"

//int can_test(int32_t channel, uint8_t mode, const void *param, int *result);
//int can_init(int32_t channel, uint8_t mode, const void *param);
//int can_exit(int handle);
//int can_kill(int handle);
//int can_start(int handle, const can_bitrate_t *bitrate);
//int can_reset(int handle);
//int can_write(int handle, const can_message_t *message, uint16_t timeout);
//int can_read(int handle, can_message_t *message, uint16_t timeout);
//int can_status(int handle, uint8_t *status);
//int can_busload(int handle, uint8_t *load, uint8_t *status);
//int can_bitrate(int handle, can_bitrate_t *bitrate, can_speed_t *speed);
//int can_property(int handle, uint16_t param, void *value, uint32_t nbyte);
//char *can_hardware(int handle);
//char* can_version(void);

can_test_fp can_test;
can_init_fp can_init;
can_exit_fp can_exit;
can_kill_fp can_kill;
can_start_fp can_start;
can_reset_fp can_reset;
can_write_fp can_write;
can_read_fp can_read;
can_status_fp can_status;
can_busload_fp can_busload;
can_bitrate_fp can_bitrate;
can_property_fp can_property;
can_hardware_fp can_hardware;
can_version_fp can_version;

static void *handler = NULL;
uint32_t comm_device_cnt = 0;

enum COMM_ERROR_CODES comm_init(char *error_code) {
    handler = dlopen("uvcantou.dylib", RTLD_LAZY);

    if (handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
        return COMM_LIB_ERROR_NULL;
    }

    can_test = dlsym(handler, "can_test");
    if (can_test == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_init = dlsym(handler, "can_init");
    if (can_init == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_exit = dlsym(handler, "can_exit");
    if (can_exit == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_kill = dlsym(handler, "can_kill");
    if (can_kill == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_start = dlsym(handler, "can_start");
    if (can_start == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_reset = dlsym(handler, "can_reset");
    if (can_reset == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_write = dlsym(handler, "can_write");
    if (can_write == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_read = dlsym(handler, "can_read");
    if (can_read == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_status = dlsym(handler, "can_status");
    if (can_status == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_busload = dlsym(handler, "can_busload");
    if (can_busload == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_bitrate = dlsym(handler, "can_bitrate");
    if (can_bitrate == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_property = dlsym(handler, "can_property");
    if (can_property == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_hardware = dlsym(handler, "can_hardware");
    if (can_hardware == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_version = dlsym(handler, "can_version");
    if (can_version == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_deinit(void) {
    if (handler == NULL) {
        return COMM_LIB_ERROR;
    }
    dlclose(handler);
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_get_device_list(struct COMM_DEVICE *comm_devices, uint32_t *num_devices) {
    int32_t state, rc = 0;
    uint32_t devices_found = 0;
    struct SChannelInfo info = {};
    can_mode_t opMode = {};
    opMode.byte = CANMODE_DEFAULT;
    int m_handle = -1;

    if (comm_devices == NULL || num_devices == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    if (handler == NULL) {
        if (comm_init(NULL) != COMM_SUCCESS) {
            return COMM_LIB_ERROR_NULL;
        }
    }

    *num_devices = 0;
    for (uint32_t x = 0; x < CAN_MAX_HANDLES; x++) {
        if (x == 0)
            rc = can_property((-1), CANPROP_SET_FIRST_CHANNEL, NULL, 0U);
        else
            rc = can_property((-1), CANPROP_SET_NEXT_CHANNEL, NULL, 0U);

        if (CANERR_NOERROR == rc) {
            can_property((-1), CANPROP_GET_CHANNEL_NO, (void *) &info.m_nChannelNo,
                         sizeof(int32_t));
            can_property((-1), CANPROP_GET_CHANNEL_NAME, (void *) &info.m_szDeviceName,
                         CANPROP_MAX_BUFFER_SIZE);
            can_property((-1), CANPROP_GET_CHANNEL_VENDOR_NAME, (void *) &info.m_szVendorName,
                         CANPROP_MAX_BUFFER_SIZE);
            rc = can_test(info.m_nChannelNo, opMode.byte, NULL, &state);
            //m_serial = can_hardware(info.m_nChannelNo);
        }

        if ((0 <= rc) && (state == CANBRD_PRESENT)) {
            devices_found++;
            comm_devices[x].id = info.m_nChannelNo;
            comm_devices[x].manufacturer = RUSOKU;
            comm_devices[x].device_model = TOUCAN;
            strcpy(comm_devices[x].manufacturer_str, info.m_szVendorName);
            strcpy(comm_devices[x].device_model_str, info.m_szDeviceName);

            m_handle = can_init(info.m_nChannelNo, 0, NULL);
            can_property(m_handle, TOUCAN_GET_SERIAL_NUMBER, &info.m_nSerialNumber, sizeof(int32_t));
            char string[CANPROP_MAX_BUFFER_SIZE] = "(unknown)";
            snprintf(string, 10, "%08X", info.m_nSerialNumber);
            can_exit(m_handle);
        }
    }
    *num_devices = devices_found;
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface) {
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface) {
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle, uint32_t *frame_cnt) {
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_read_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_write_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    return COMM_SUCCESS;
}
