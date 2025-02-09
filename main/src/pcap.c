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

#include "../inc/pcap.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

uint16_t randRange(int n) {
    uint16_t limit;
    uint16_t r;
    limit = RAND_MAX - (RAND_MAX % n);
    while ((r = rand()) >= limit);
    return r % n;
}

uint32_t swap_endianness(uint32_t bytes, int bit) {
    bytes = (bytes & 0xFFFF0000) >> 16 | (bytes & 0x0000FFFF) << 16;
    bytes = (bytes & 0xFF00FF00) >> 8 | (bytes & 0x00FF00FF) << 8;
    if (bit == 1) {
        bytes = (bytes & 0xF0F0F0F0) >> 4 | (bytes & 0x0F0F0F0F) << 4;
        bytes = (bytes & 0xCCCCCCCC) >> 2 | (bytes & 0x33333333) << 2;
        bytes = (bytes & 0xAAAAAAAA) >> 1 | (bytes & 0x55555555) << 1;
    } else if (bit == 16) bytes = bytes >> 16;
    return bytes;
}

/*********************************** PCAP file header ***************************************/
void pcap_prepare_file_header(struct PCAP_FILE_HEADER *pcap_global_fh, uint32_t dlt) {
    pcap_global_fh->magic = 0xA1B23C4D; //0xA1B2C3D4; uS
    pcap_global_fh->version_major = 2;
    pcap_global_fh->version_minor = 4;
    pcap_global_fh->thiszone = 0;
    pcap_global_fh->sigfigs = 0;
    pcap_global_fh->snaplen = 2048; //TODO: is buffer size correct ?
    pcap_global_fh->linktype = dlt;
}

struct PCAP_PACKET_RECORD_HEADER pcap_prepare_pkt_header(uint32_t pkt_cap_len, uint32_t pkt_len) {
    static struct PCAP_PACKET_RECORD_HEADER pcap_packet_header = {};
    struct timespec spec = {};

    clock_gettime(CLOCK_REALTIME, &spec);

    pcap_packet_header.sec = spec.tv_sec;
    pcap_packet_header.usec = spec.tv_nsec; //round(spec.tv_nsec / 1.0e6);

    pcap_packet_header.caplen = pkt_cap_len;
    pcap_packet_header.len = pkt_len;
    return pcap_packet_header;
}

/************************************* LINKTYPE_LINUX_SLL ***************************/
struct PCAP_LINKTYPE_LINUX_SLL_HEADER pcap_prepare_sll_header(uint16_t pkttype) {
    static struct PCAP_LINKTYPE_LINUX_SLL_HEADER sll_header = {};
    sll_header.sll_pkttype = swap_endianness(pkttype, 16);
    sll_header.sll_hatype = swap_endianness(HATYPE_ARPHRD_NONE, 16);
    sll_header.sll_halen = 0;
    for (int i = 0; i < SLL_ADDRLEN; i++)
        sll_header.sll_addr[i] = 0xFF;
    sll_header.sll_protocol = swap_endianness(SLL_PROTOCOL_TYPE_CAN, 16);
    return sll_header;
}

struct PCAP_LINKTYPE_CAN_CC_SOCKETCAN pcap_prepare_socketcan_linktype(struct COMM_CAN_MSG *can_frame) {
    static struct PCAP_LINKTYPE_CAN_CC_SOCKETCAN socketcan_cc_frame = {};
    if (can_frame->flags & CAN_FLAG_EXTENDED) {
        socketcan_cc_frame.can_id = swap_endianness(can_frame->id, 0) | swap_endianness(0x80000000, 0);
    } else {
        socketcan_cc_frame.can_id = swap_endianness(can_frame->id, 0);
    }
    socketcan_cc_frame.can_payload_length = can_frame->length;
    socketcan_cc_frame.can_dlc = can_frame->length;
    memcpy((void *) &socketcan_cc_frame.data[0], (void *) &can_frame->data[0], 8);
    return socketcan_cc_frame;
}

/*
void get_timestamp(uint32_t *seconds, uint32_t *msec)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    SYSTEMTIME timestamp;
    LPSYSTEMTIME pTime = &timestamp;s
    GetSystemTime(pTime);
    tm.tm_year = pTime->wYear - 1900;
    tm.tm_mon = pTime->wMonth - 1;
    tm.tm_mday = pTime->wDay;

    tm.tm_hour = pTime->wHour;
    tm.tm_min = pTime->wMinute;
    tm.tm_sec = pTime->wSecond;
    tm.tm_isdst = -1;
    *seconds = uint32_t(mktime(&tm));
    *msec = uint32_t(pTime->wMilliseconds)*1000;
}
*/
