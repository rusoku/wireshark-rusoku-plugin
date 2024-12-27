//
// Created by Gediminas Simanskis on 26/12/2024.
//

#ifndef MAIN_H
#define MAIN_H

#define EXTCAP_INTERFACES       0
#define EXTCAP_INTERFACE        1
#define EXTCAP_VERSION          2
#define EXTCAP_CONFIG           3
#define EXTCAP_CAPTURE_FILTER   4
#define FIFO                    5
#define EXTCAP_CONTROL_IN       6
#define EXTCAP_CONTROL_OUT      7
#define EXTCAP_RELOAD_OPTION    8
#define EXTCAP_DLTS             9
#define PARAMETER_BITRATE       10
#define PARAMETER_SILENT        11
#define PARAMETER_LOOPBACK      12
#define CAPTURE                 13
#define LOG_LEVEL               14
#define LOG_FILE                15
#define GOPT_LAST_OPT           16


struct _interface_parameters{
    int bitrate;
    int silent;
    int loopback;
};

#endif //MAIN_H
