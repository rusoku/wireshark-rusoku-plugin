//
// Created by Gediminas Simanskis on 29/12/2024.
//

#include "../inc/pcap.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include <pthread.h>
//#include <math.h>
//#include "../inc/comm_base.h"


/*
void print_current_time_with_ms (void)
{
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s++;
        ms = 0;
    }
}
*/

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
//Prepare PCAP file header
void pcap_prepare_file_header(struct PCAP_FILE_HEADER *pcap_global_fh, uint32_t dlt) {
    pcap_global_fh->magic = 0xA1B23C4D; //0xA1B2C3D4; uS
    pcap_global_fh->version_major = 2;
    pcap_global_fh->version_minor = 4;
    pcap_global_fh->thiszone = 0;
    pcap_global_fh->sigfigs = 0;
    pcap_global_fh->snaplen = 2048; //TODO: is buffer size correct ?
    pcap_global_fh->linktype = dlt;
}

//struct PCAP_PACKET_RECORD_HEADER init_pcap_pkt_header(structCanalMsg *canal_frame, pcappkt_can *pcap_frame)
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

//PACK__ struct PCAP_LINKTYPE_CAN_SOCKETCAN {
//    uint32_t can_id_flags;					/* can id */
//    uint8_t length;          	            /* payload length */
//    uint8_t fd_flags;						/* fd lags */
//    uint8_t reserved1;			            /* reserved1 */
//    uint8_t reserved2;  					/* reserved2 */
//}__PACK;

// CAN flags values
//#define CAN_FLAG_STANDARD    0x00000000L
//#define CAN_FLAG_EXTENDED    0x00000001L
//#define CAN_FLAG_RTR         0x00000002L
//#define CAN_FLAG_STATUS      0x00000004L
//#define CAN_FLAG_CAN_FD      0x00000008L
//#define CAN_FLAG_CAN_XT      0x00000010L
//#define CAN_FLAG_SEND        0x80000000L

// CAN error codes
//#define CAN_STATUS_OK 0x00    // normal condition.
//#define CAN_STATUS_OVERRUN 0x01
//#define CAN_STATUS_BUSLIGHT 0x02
//#define CAN_STATUS_BUSHEAVY 0x03
//#define CAN_STATUS_BUSOFF   0x04
/*
struct COMM_CAN_MSG {
    uint32_t id;
    uint32_t error_status; // error counters (if CAN_FLAG_STATUS is set)
    uint32_t error_code; // error codes
    uint32_t flags; // can flags: STD, EXT, CAN FD etc.
    uint8_t length;
    uint8_t data[64];
};*/

struct PCAP_LINKTYPE_CAN_CC_SOCKETCAN prepare_socketcan_linktype_from_canframe(struct COMM_CAN_MSG *can_frame) {
    static struct PCAP_LINKTYPE_CAN_CC_SOCKETCAN socketcan_cc_frame = {};
    if (can_frame->flags & CAN_FLAG_EXTENDED) {
        socketcan_cc_frame.can_id = swap_endianness(can_frame->id, 0) | swap_endianness(0x80000000, 0);
    } else {
        socketcan_cc_frame.can_id = swap_endianness(can_frame->id, 0);
    }
    socketcan_cc_frame.payload_length = can_frame->length;
    memcpy((void *) &socketcan_cc_frame.data[0], (void *) &can_frame->data[0], 8);
    return socketcan_cc_frame;
}
/*
int convert_canal_to_socketcan(can_frame* can_frame, structCanalMsg* canal_frame)
{
    memset(can_frame, 0, LEN_CAN_FRAME);
    can_frame->can_id = swap_endianness((uint16_t)canal_frame->flags,1) | canal_frame->id;
    can_frame->can_dlc = canal_frame->sizeData;
    memcpy(can_frame->data, canal_frame->data, CAN_MAX_DLEN);
    return 0;
}
*/
/*
void get_timestamp(uint32_t *seconds, uint32_t *msec)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    SYSTEMTIME timestamp;
    LPSYSTEMTIME pTime = &timestamp;
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
