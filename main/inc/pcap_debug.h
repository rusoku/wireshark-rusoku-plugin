//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2025 Gediminas Simanskis, Rusoku technologijos UAB, Vilnius, Lithuania (info@rusoku.com)
//  All rights reserved.
//
//  GNU General Public License v3.0 or later:
//  Rusoku wireshark CAN bus adapters extcap plugin is free software: you can redistribute it and/or modify
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

#ifndef PCAP_DEBUG_H
#define PCAP_DEBUG_H

#include <stdio.h>
#include <stdarg.h>

extern int can_dbg_printf(FILE *file, const char *format, ...);

#ifndef NDEBUG
#define PCAP_DEBUG(...)  can_dbg_printf(stderr, __VA_ARGS__)
#else
    #define PCAP_DEBUG(...)
#endif

#endif //PCAP_DEBUG_H
