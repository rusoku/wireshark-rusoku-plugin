//
// Created by gedsi on 2025-01-06.
//

#include "../../main/inc/comm_base.h"
#include "../inc/comm_rusoku_win.h"

CanalOpen_fp CanalOpen;
CanalClose_fp CanalClose;
CanalGetDeviceList_fp CanalGetDeviceList;
CanalGetSerialNumber_fp CanalGetSerialNumber;
CanalDataAvailable_fp CanalDataAvailable;
CanalReceive_fp CanalReceive;
CanalSend_fp CanalSend;
CanalGetStatus_fp CanalGetStatus;

static void *handler = NULL;

enum COMM_ERROR_CODES comm_init(char *error_code) {
    handler = dlopen("canal.dll", RTLD_LAZY);

    if (handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
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
    /*
    struct CANAL_DEV_INFO {
        uint16_t DeviceId;
        uint16_t vid;
        uint16_t pid;
        char SerialNumber[16];
    };

    struct CANAL_DEV_LIST {
        struct CANAL_DEV_INFO canDevInfo[CANAL_DEVLIST_SIZE_MAX];
        uint16_t canDevCount;
    };

    typedef struct COMM_DEVICE {
        uint16_t id;
        enum COMM_MANUFACTURER manufacturer;
        enum COMM_DEVICE_TYPE device_type;
        char model[16];
        char serial[16];
    }COMM_DEVICES[8];
  */

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
        //printf("SERIAL %s\n", CanalDevList.canDevInfo[index].SerialNumber);
        strcpy(comm_devices[index].serial, CanalDevList.canDevInfo[index].SerialNumber);
        comm_devices[index].id = index;
    }

    return COMM_SUCCESS;
}
