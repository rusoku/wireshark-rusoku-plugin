//
// Created by RTS on 2025-01-15.
//

#ifndef COMM_RUSOKU_WIN_H
#define COMM_RUSOKU_WIN_H

#include "../../main/inc/main.h"

#define CANAL_DEVLIST_SIZE_MAX 64

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
    uint16_t DeviceId;
    uint16_t vid;
    uint16_t pid;
    char SerialNumber[16];
};

struct struct_CANAL_DEV_LIST {
    struct CANAL_DEV_INFO canDevInfo[CANAL_DEVLIST_SIZE_MAX];
    uint16_t canDevCount;
};

typedef int32_t (*CanalOpen_fp)(const char *pConfigStr, unsigned long flags);

typedef void (*CanalClose_fp)(uint32_t handle);

typedef int32_t (*CanalGetDeviceList_fp)(uint32_t handle, struct struct_CANAL_DEV_LIST *canal_dev_list, uint16_t canal_dev_list_size);

typedef int32_t (*CanalGetSerialNumber_fp)(uint32_t handle, char *pSerialNumberStr);

typedef int32_t (*CanalDataAvailable_fp)(uint32_t handle);

typedef int32_t (*CanalReceive_fp)(uint32_t handle, struct CANAL_MSG *pCanMsg);

typedef int32_t (*CanalSend_fp)(uint32_t handle, struct CANAL_MSG *pCanMsg);

typedef int32_t (*CanalGetStatus_fp)(uint32_t handle, struct CANAL_STATUS *pCanStatus);

#endif //COMM_RUSOKU_WIN_H
