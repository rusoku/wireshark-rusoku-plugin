//
// Created by RTS on 2025-02-05.
//


#include "../inc/pcap_param.h"

uint8_t onCapture = 0;
int8_t interface = -1;

struct INTERFACE_PARAMETERS pcap_main_interface_parameters = {
    .interface_nr = -1,
    .dlt_type = 1,
    .bitrate = 125,
    .bitrate_data = 125,
    .options = 0
};
