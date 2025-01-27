//
// Created by gedsi on 2025-01-21.
//
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
