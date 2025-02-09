//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2025 Gediminas Simanskis, Rusoku technologijos UAB, Vilnius, Lithuania (info@rusoku.com)
//  All rights reserved.
//
//  GNU General Public License v3.0 or later:
//  Rusoku wireshark extcap plugin is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Rusoku wireshark extcap plugin is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Rusoku wireshark extcap plugin.If not, see <https://www.gnu.org/licenses/>.

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
