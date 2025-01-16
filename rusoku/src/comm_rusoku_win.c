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

enum ERROR_CODES comm_init(char *error_code) {
    handler = dlopen("canal.dll", RTLD_NOW);

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

enum ERROR_CODES comm_deinit(void) {
    dlclose(handler);
    handler = NULL;
    return COMM_SUCCESS;
};

enum ERROR_CODES comm_get_device_list(struct COMM_DEVICE *comm_devices, uint32_t *num_devices) {
    if (comm_devices == NULL || num_devices == NULL) {
        return COMM_LIB_ERROR_NULL;
    }

    if (handler == NULL) {
        comm_init(NULL);
    }

    return COMM_SUCCESS;
}
