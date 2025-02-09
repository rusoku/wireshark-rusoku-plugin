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

#ifndef MAIN_H
#define MAIN_H

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>
//#include <time.h>
//#include <stdint.h>
//#include <dlfcn.h>
//#include <pthread.h>
//#include "pcap.h"

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

//extern FILE *fpCAN_DEBUG;

#endif //MAIN_H
