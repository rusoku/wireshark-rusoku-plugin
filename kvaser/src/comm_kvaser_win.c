//
// Created by gedsi on 2025-01-06.
//

#include "../inc/comm_kvaser_win.h"
#include "../../main/inc/comm_base.h"


static uint32_t hcanal;

typedef uint32_t (*CanalOpen_fp)(const char *pConfigStr, unsigned long flags);

CanalOpen_fp CanalOpen;

int16_t comm_init_rusoku(void *handler, char *error_code){

    if (handler == NULL) {
        return COMM_LIB_ERROR;
    }

#if defined __WIN32__ || defined __WIN64__
    handler = dlopen("canal.dll", RTLD_NOW);
#elif defined __APPLE__
    handler = dlopen("uvcantou.dylib", RTLD_NOW);
#else
    handler = dlopen("uvcantou.so", RTLD_NOW);
#endif

    if (handler == NULL) {
        if (error_code != NULL) {
            error_code = dlerror();
        }
    }

    CanalOpen = dlsym(handler, "CanalOpen");
    //CanalOpen("0;00005499;250", 0);

    return  COMM_SUCCESS;
};

int16_t comm_deinit_rusoku(void *handler){

    if (handler == NULL) {
        return COMM_LIB_ERROR;
    }
    dlclose(handler);
    return  COMM_SUCCESS;
};