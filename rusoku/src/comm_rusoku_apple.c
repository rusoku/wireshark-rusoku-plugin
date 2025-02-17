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
#include "../inc/CANAPI_Types.h"
#include "../inc/comm_rusoku_apple.h"

#include <unistd.h>

#include "../../main/inc/comm_base.h"
#include "../../main/inc/pcap_debug.h"

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

static void *lib_handler = NULL;
static int device_handle = -1;
uint32_t comm_device_cnt = 0;

enum COMM_ERROR_CODES comm_init(char *error_code) {
    lib_handler = dlopen("libUVCANTOU.dylib", RTLD_LAZY);

    if (lib_handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
        return COMM_LIB_ERROR_NULL;
    }

    can_test = dlsym(lib_handler, "can_test");
    if (can_test == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_init = dlsym(lib_handler, "can_init");
    if (can_init == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_exit = dlsym(lib_handler, "can_exit");
    if (can_exit == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_kill = dlsym(lib_handler, "can_kill");
    if (can_kill == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_start = dlsym(lib_handler, "can_start");
    if (can_start == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_reset = dlsym(lib_handler, "can_reset");
    if (can_reset == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_write = dlsym(lib_handler, "can_write");
    if (can_write == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_read = dlsym(lib_handler, "can_read");
    if (can_read == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_status = dlsym(lib_handler, "can_status");
    if (can_status == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_busload = dlsym(lib_handler, "can_busload");
    if (can_busload == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_bitrate = dlsym(lib_handler, "can_bitrate");
    if (can_bitrate == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_property = dlsym(lib_handler, "can_property");
    if (can_property == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_hardware = dlsym(lib_handler, "can_hardware");
    if (can_hardware == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    can_version = dlsym(lib_handler, "can_version");
    if (can_version == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_deinit(void) {
    if (lib_handler != NULL) {
        dlclose(lib_handler);
        lib_handler = NULL;
    }
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

    if (lib_handler == NULL) {
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
            char serial_str[CANPROP_MAX_BUFFER_SIZE] = "(unknown)";
            snprintf(serial_str, 10, "%08X", info.m_nSerialNumber);
            strcpy(comm_devices[x].serial, serial_str);
            can_exit(m_handle);
        }
    }
    *num_devices = devices_found;
    comm_deinit();
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface) {
    can_mode_t can_mode;
    can_bitrate_t bitrate;
    can_mode.byte = CANMODE_DEFAULT;

    if (comm_interface.options && INTERFACE_PARAMETER_OPTION_SILENT) {
        can_mode.mon = true;
    }

    if (lib_handler == NULL) {
        if (comm_init(NULL) != COMM_SUCCESS) {
            return COMM_LIB_ERROR_NULL;
        }
    }

    device_handle = can_init(comm_interface.interface_nr, can_mode.byte, NULL);

    if (device_handle < CANERR_NOERROR) {
        return COMM_DEVICE_INIT_ERROR;
    }
    switch (comm_interface.bitrate) {
        case 1000000: bitrate.index = (int32_t) CANBTR_INDEX_1M;
            break;
        case 800000: bitrate.index = (int32_t) CANBTR_INDEX_800K;
            break;
        case 500000: bitrate.index = (int32_t) CANBTR_INDEX_500K;
            break;
        case 250000: bitrate.index = (int32_t) CANBTR_INDEX_250K;
            break;
        case 125000: bitrate.index = (int32_t) CANBTR_INDEX_125K;
            break;
        case 100000: bitrate.index = (int32_t) CANBTR_INDEX_100K;
            break;
        case 50000: bitrate.index = (int32_t) CANBTR_INDEX_50K;
            break;
        case 20000: bitrate.index = (int32_t) CANBTR_INDEX_20K;
            break;
        case 10000: bitrate.index = (int32_t) CANBTR_INDEX_10K;
            break;
        default: bitrate.index = (int32_t) CANBTR_INDEX_250K;
            break;
    }
    if (can_start(device_handle, &bitrate) < CANERR_NOERROR) {
        return COMM_DEVICE_INIT_ERROR;
    }
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface) {
    can_exit(device_handle);
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle, uint32_t *frame_cnt) {
    if (frame_cnt == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    *frame_cnt = 1;
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_read_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    if (comm_can_msg == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    comm_can_msg->id = 0x123;
    comm_can_msg->flags = 0;
    comm_can_msg->length = 2;
    comm_can_msg->data[0] = 0x12;
    comm_can_msg->data[1] = 0x34;

    usleep(1000);

    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_write_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    if (comm_can_msg == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    return COMM_SUCCESS;
}
