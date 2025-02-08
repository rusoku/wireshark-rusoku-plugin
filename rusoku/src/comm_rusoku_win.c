//
// Created by gedsi on 2025-01-06.
//

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "../../main/inc/comm_base.h"
#include "../../main/inc/pcap_param.h"
#include "../inc/comm_rusoku_win.h"
#include "../../main/inc/pcap_debug.h"

uint32_t comm_device_cnt = 0;

CanalOpen_fp CanalOpen;
CanalClose_fp CanalClose;
CanalGetDeviceList_fp CanalGetDeviceList;
CanalGetSerialNumber_fp CanalGetSerialNumber;
CanalDataAvailable_fp CanalDataAvailable;
CanalReceive_fp CanalReceive;
CanalSend_fp CanalSend;
CanalGetStatus_fp CanalGetStatus;

static void *handler = NULL;
static int64_t canal_handler = -1;

enum COMM_ERROR_CODES comm_init(char *error_code) {
    handler = dlopen("canal.dll", RTLD_LAZY);

    if (handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
        return COMM_LIB_ERROR_NULL;
    }

    CanalOpen = dlsym(handler, "CanalOpen");
    if (CanalOpen == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalClose = dlsym(handler, "CanalClose");
    if (CanalClose == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalGetDeviceList = dlsym(handler, "CanalGetDeviceList");
    if (CanalGetDeviceList == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalGetSerialNumber = dlsym(handler, "CanalGetSerialNumber");
    if (CanalGetSerialNumber == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalDataAvailable = dlsym(handler, "CanalDataAvailable");
    if (CanalDataAvailable == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalReceive = dlsym(handler, "CanalReceive");
    if (CanalReceive == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalSend = dlsym(handler, "CanalSend");
    if (CanalSend == NULL) {
        return COMM_LIB_ERROR_NULL;
    }
    CanalGetStatus = dlsym(handler, "CanalGetStatus");
    if (CanalGetStatus == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_deinit(void) {
    dlclose(handler);
    handler = NULL;
    return COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_get_device_list(struct COMM_DEVICE *comm_devices, uint32_t *num_devices) {
    if (comm_devices == NULL || num_devices == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    if (handler == NULL) {
        if (comm_init(NULL) != COMM_SUCCESS) {
            return COMM_LIB_ERROR_NULL;
        }
    }

    //canal_dev_list CanalDevList; // CANAL device list
    struct CANAL_DEV_LIST CanalDevList;
    if (CanalGetDeviceList(&CanalDevList, 8) != CANAL_ERROR_SUCCESS) {
        return COMM_LIB_ERROR;
    }

    *num_devices = CanalDevList.canDevCount;

    for (uint8_t index = 0; index < CanalDevList.canDevCount; index++) {
        strcpy(comm_devices[index].serial, CanalDevList.canDevInfo[index].SerialNumber);
        strcpy(comm_devices[index].manufacturer_str, MANUFACTURER);
        strcpy(comm_devices[index].device_model_str, MODEL_TOUCAN);
        comm_devices[index].id = index;
    }

    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface) {
    char buffer[128];

    if (comm_get_device_list(comm_devices, &comm_device_cnt) != COMM_SUCCESS) {
        return COMM_DEVICE_INIT_ERROR;
    }

    strcpy(comm_interface.serial_str, comm_devices[comm_interface.interface_nr].serial);
    sprintf(buffer, "0;%s;%d", comm_interface.serial_str, comm_interface.bitrate);
    PCAP_DEBUG("comm_rusoku: open_device %s\n", buffer);

    canal_handler = CanalOpen(buffer, 0);
    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS comm_interface) {
    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }

    CanalClose(canal_handler);

    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_read_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    struct CANAL_MSG CanalMsg = {};

    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }
    if (comm_can_msg == NULL) {
        return COMM_POINTER_ERROR;
    }

    if (CanalDataAvailable(canal_handler)) {
        if (CanalReceive(canal_handler, &CanalMsg) != CANAL_ERROR_SUCCESS) {
            return COMM_DEVICE_IO_ERROR;
        }
    }
    comm_can_msg->id = CanalMsg.id;
    comm_can_msg->length = CanalMsg.sizeData;

    if (CanalMsg.flags & CANAL_IDFLAG_EXTENDED) {
        comm_can_msg->flags = CanalMsg.flags & CANAL_IDFLAG_EXTENDED;
    }
    memcpy(comm_can_msg->data, CanalMsg.data, CanalMsg.sizeData);
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle, uint32_t *frame_cnt) {

    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }
    if (frame_cnt == NULL) {
        return COMM_POINTER_ERROR;
    }
    *frame_cnt = CanalDataAvailable(canal_handler);
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_write_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg) {
    struct CANAL_MSG CanalMsg = {};

    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }
    if (comm_can_msg == NULL) {
        return COMM_POINTER_ERROR;
    }
    CanalMsg.id = comm_can_msg->id;
    CanalMsg.sizeData = comm_can_msg->length;
    memcpy(CanalMsg.data, comm_can_msg->data, comm_can_msg->length);

    if (comm_can_msg->flags & CAN_FLAG_EXTENDED) {
        CanalMsg.flags |= CANAL_IDFLAG_EXTENDED;
    }

    if (CanalSend(canal_handler, &CanalMsg) != CANAL_ERROR_SUCCESS) {
        return COMM_DEVICE_IO_ERROR;
    }
    return COMM_SUCCESS;
}
