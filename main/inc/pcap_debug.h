//
// Created by gedsi on 2025-01-21.
//

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
