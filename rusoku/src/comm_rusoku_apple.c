//
// Created by gedsi on 2025-01-06.
//

#include "../inc/comm_rusoku_apple.h"
#include "../../main/inc/comm_base.h"

//typedef uint32_t (*CanalOpen_fp)(const char *pConfigStr, unsigned long flags);

static void *handler = NULL;

enum COMM_ERROR_CODES comm_init(char *error_code) {
    handler = dlopen("uvcantou.dylib", RTLD_LAZY);

    if (handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
        return COMM_LIB_ERROR_NULL;
    }
    return  COMM_SUCCESS;
};

enum COMM_ERROR_CODES comm_deinit(void){

    if (handler == NULL) {
        return COMM_LIB_ERROR;
    }
    dlclose(handler);
    return  COMM_SUCCESS;
};
