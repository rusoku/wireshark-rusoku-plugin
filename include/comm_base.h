//
// Created by gedsi on 2025-01-06.
//

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "../include/main.h"

enum ERROR_CODES {
    COMM_SUCCESS = 0,
    COMM_LIB_ERROR = -1,
};

enum COMM_MANUFACTURER {
    RUSOKU = 0,
    KVASER,
    PEAK
};

enum COMM_DEVICE_TYPE {
    TOUCAN = 0
};

struct COMM_DEVICE {
    uint16_t id;
    uint16_t type;
    uint16_t manufacturer;
    char model[128];
    char serial[128];
    uint16_t (*comm_init)(void);
    uint16_t (*comm_get_device_list)(void);
};

#endif //COMMUNICATIONS_H
