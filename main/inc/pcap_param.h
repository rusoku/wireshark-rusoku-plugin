//
// Created by RTS on 2025-02-05.
//

#ifndef PCAP_PARAM_H
#define PCAP_PARAM_H

#include <stdint.h>

struct INTERFACE_PARAMETERS {
    int8_t interface_nr;
    uint8_t dlt_type;
    char serial_str[16];
    uint32_t bitrate;           //can bitrate
    uint32_t bitrate_data;      //can fd bitrate
    uint32_t options;           //silent, loopbach, canfd, etc.
    char *fifo_data;
    char *fifo_cntrl_out;
    char *fifo_cntrl_in;
};

extern struct INTERFACE_PARAMETERS  pcap_main_interface_parameters;
extern int8_t interface;
extern uint8_t onCapture;

#endif //PCAP_PARAM_H
