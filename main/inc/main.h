//
// Created by Gediminas Simanskis on 26/12/2024.
//

#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <dlfcn.h>
#include <pthread.h>
#include "pcap.h"
#include "pcap_debug.h"

#define PCAP_TOTAL_DEVICES      8
#define EXTCAP_INTERFACE        0
#define EXTCAP_CAPTURE_FILTER   1
#define FIFO                    2
#define EXTCAP_CONTROL_IN       3
#define EXTCAP_CONTROL_OUT      4
#define EXTCAP_RELOAD_OPTION    5
#define PARAMETER_BITRATE       6
#define PARAMETER_BITRATE_DATA  7
#define PARAMETER_SILENT        8
#define PARAMETER_LOOPBACK      9
#define PARAMETER_CANFD         10
#define LOG_LEVEL               11
#define LOG_FILE                12
#define EXTCAP_INTERFACES       13
#define EXTCAP_VERSION          14
#define EXTCAP_CONFIG           15
#define EXTCAP_DLTS             16
#define PARAMETER_DLT_LINKTYPE  17
#define CAPTURE                 18
#define GOPT_LAST_OPT           19

#define INTERFACE_PARAMETER_OPTION_SILENT     0x00000001L
#define INTERFACE_PARAMETER_OPTION_LOOPBACK   0x00000002L
#define INTERFACE_PARAMETER_OPTION_CANFD      0x00000004L

struct INTERFACE_PARAMETERS {
    int8_t interface_nr;
    uint8_t dlt_type;
    char serial_str[16];
    uint32_t bitrate; //can bitrate
    uint32_t bitrate_data; //can fd bitrate
    uint32_t options; //silent, loopbach, canfd, etc.
    char *fifo_data;
    char *fifo_cntrl_out;
    char *fifo_cntrl_in;
};

extern struct INTERFACE_PARAMETERS interface_parameters;
extern int8_t interface;
extern uint8_t onCapture;

//extern FILE *fpCAN_DEBUG;

#endif //MAIN_H
