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

#include "../inc/pcap_debug.h"
#include <stdio.h>

#ifdef __w64
#include <windows.h>
#endif

#ifdef __w64
int can_dbg_printf(FILE *file, const char *format,...) {
    int res;
    va_list argp;
    va_start(argp, format);
    char dbg_out[8192];
    vsprintf_s(dbg_out, sizeof(dbg_out) / sizeof(dbg_out[0]), format, argp);
    va_end(argp);
    OutputDebugStringA(dbg_out);
    return res;
}
#elif __APPLE__
int can_dbg_printf(FILE *file, const char *format,...) {
    int res;
    va_list args;
    va_start(args, format);
    res = vfprintf(file, format, args);
    va_end(args);
    fflush(file);
    return res;
}
#endif
