//
// Created by gedsi on 2025-01-06.
//

#include "../../main/inc/comm_base.h"
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

enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface) {
    char buffer[128];

    if (comm_get_device_list(comm_devices, &comm_device_cnt) != COMM_SUCCESS) {
        return COMM_DEVICE_INIT_ERROR;
    }

    strcpy(interface.serial_str, comm_devices[interface.interface_nr].serial);
    sprintf(buffer, "0;%s;%d", interface.serial_str, interface.bitrate);
    PCAP_DEBUG("comm_rusoku: open_device %s\n", buffer);

    canal_handler = CanalOpen(buffer, 0);
    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }
    return COMM_SUCCESS;
}

enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface) {
    if (canal_handler < 0) {
        return COMM_DEVICE_INIT_ERROR;
    }

    CanalClose(canal_handler);

    return COMM_SUCCESS;
}

/*
enum COMM_ERROR_CODES comm_init(char *error_code);
enum COMM_ERROR_CODES comm_deinit(void);
enum COMM_ERROR_CODES comm_get_device_list(struct COMM_DEVICE *comm_devices, uint32_t *num_devices);
enum COMM_ERROR_CODES comm_open_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface);
enum COMM_ERROR_CODES comm_close_device(COMM_DEV_HANDLE dev_handle, struct INTERFACE_PARAMETERS interface);
enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle,uint32_t *frame_cnt);
enum COMM_ERROR_CODES comm_read_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg);
enum COMM_ERROR_CODES comm_write_frame(COMM_DEV_HANDLE comm_dev_handle, struct COMM_CAN_MSG *comm_can_msg);

struct COMM_CAN_MSG {
    uint32_t id;
    uint32_t error_status; // error counters (if CAN_FLAG_STATUS is set)
    uint32_t error_code; // error codes
    uint32_t flags; // can flags: STD, EXT, CAN FD etc.
    uint8_t length;
    uint8_t data[64];
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
**************** canal ************************
/// ID flags
#define CANAL_IDFLAG_STANDARD       0x00000000	// Standard message id (11-bit)
#define CANAL_IDFLAG_EXTENDED       0x00000001	// Extended message id (29-bit)
#define CANAL_IDFLAG_RTR            0x00000002	// RTR-Frame
#define CANAL_IDFLAG_STATUS         0x00000004	// This package is a status indication (id holds error code)
#define CANAL_IDFLAG_SEND           0x80000000  // Reserved for use by application software to indicate send

struct CANAL_MSG {
    uint32_t flags; // CAN message flags
    uint32_t obid; // Used by driver for channel info etc.
    uint32_t id; // CAN id (11-bit or 29-bit)
    uint8_t sizeData; // Data size 0-8
    uint8_t data[8]; // CAN Data
    uint32_t timestamp; // Relative time stamp for package in microseconds
};
*/
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

enum COMM_ERROR_CODES comm_get_device_data_available(COMM_DEV_HANDLE comm_dev_handle,uint32_t *frame_cnt) {

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
