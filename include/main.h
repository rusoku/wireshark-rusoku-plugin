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

#include "pcap.h"
#include "control_packet_headers.h"

#define EXTCAP_INTERFACE        0
#define EXTCAP_CAPTURE_FILTER   1
#define FIFO                    2
#define EXTCAP_CONTROL_IN       3
#define EXTCAP_CONTROL_OUT      4
#define EXTCAP_RELOAD_OPTION    5
#define PARAMETER_BITRATE       6
#define PARAMETER_SILENT        7
#define PARAMETER_LOOPBACK      8
#define LOG_LEVEL               9
#define LOG_FILE                10
#define EXTCAP_INTERFACES       11
#define EXTCAP_VERSION          12
#define EXTCAP_CONFIG           13
#define EXTCAP_DLTS             14
#define CAPTURE                 15
#define GOPT_LAST_OPT           16

#define INTERAFACE_PARAMETER_SILENT     1
#define INTERAFACE_PARAMETER_LOOPBACK   2

struct INTERFACE_PARAMETERS{
    char        serial[16];
    uint32_t    bitrate;
    uint32_t    options;;
};

#endif //MAIN_H
