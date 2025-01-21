//
// Created by gedsi on 2025-01-21.
//
#include "../inc/debug.h"
#include <stdio.h>
#include <windows.h>

void DebugPrintf(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    char dbg_out[8192];
    vsprintf_s(dbg_out, sizeof(dbg_out) / sizeof(dbg_out[0]), fmt, argp);
    va_end(argp);
    OutputDebugStringA(dbg_out);
}
