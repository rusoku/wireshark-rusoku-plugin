//
// Created by Gediminas Simanskis on 29/12/2024.
//

#include "../include/pcap.h"

uint16_t randRange(int n)
{
    uint16_t limit;
    uint16_t r;

    limit = RAND_MAX - (RAND_MAX % n);

    while((r = rand()) >= limit);
    return r % n;
}

uint32_t swap_endianness(uint32_t bytes, int bit) {
    bytes = (bytes & 0xFFFF0000) >> 16 | (bytes & 0x0000FFFF) << 16;
    bytes = (bytes & 0xFF00FF00) >> 8  | (bytes & 0x00FF00FF) << 8;
    if (bit == 1) {
        bytes = (bytes & 0xF0F0F0F0) >> 4  | (bytes & 0x0F0F0F0F) << 4;
        bytes = (bytes & 0xCCCCCCCC) >> 2 | (bytes & 0x33333333) << 2;
        bytes = (bytes & 0xAAAAAAAA) >> 1 | (bytes & 0x55555555) << 1;
    }
    else if (bit == 16) bytes = bytes >> 16;
    return bytes;
}

void init_pcap_file_header(struct PCAP_FILE_HEADER *pcap_global_fh) {
    pcap_global_fh->magic = 0xA1B2C3D4;
    pcap_global_fh->version_major = 2;
    pcap_global_fh->version_minor = 4;
    pcap_global_fh->thiszone = 0;
    pcap_global_fh->sigfigs = 0;
    pcap_global_fh->snaplen = 40; // TODO: is buffer correct ?
    pcap_global_fh->linktype = LINKTYPE_LINUX_SLL; // DLT 113
}

//struct PCAP_PACKET_RECORD_HEADER init_pcap_pkt_header(structCanalMsg *canal_frame, pcappkt_can *pcap_frame)
struct PCAP_PACKET_RECORD_HEADER init_pcap_pkt_header(void)
{
    struct PCAP_PACKET_RECORD_HEADER pcap_packet_header;

    //get_timestamp(&pcap_packet_header.sec, &pcap_packet_header.usec);
    pcap_packet_header.sec = 0;  // TODO: temporary, fix this
    pcap_packet_header.usec = 0; // TODO: temporary, fix this

    pcap_packet_header.caplen = PCAP_PKT_LEN;
    pcap_packet_header.len = PCAP_PKT_LEN;
    return pcap_packet_header;
}


struct PCAP_LINKTYPE_LINUX_SLL_HEADER init_sll_header(uint16_t pkttype)
{
    struct PCAP_LINKTYPE_LINUX_SLL_HEADER sll_header;
    sll_header.sll_pkttype = swap_endianness(pkttype, 16);
    sll_header.sll_hatype = swap_endianness(HATYPE_ARPHRD_NONE, 16);
    sll_header.sll_halen = 0;
    for (int i = 0; i < SLL_ADDRLEN; i++) sll_header.sll_addr[i] = 0xFF;
    sll_header.sll_protocol = swap_endianness(SLL_PROTOCOL_TYPE_CAN, 16);
    return sll_header;
}

struct SOCKETCAN_FRAME_HEADER init_rnd_fake_can_header(void)
{
    struct SOCKETCAN_FRAME_HEADER can_frame;
    can_frame.can_id = randRange(1000);
    can_frame.can_dlc = 8;

    can_frame.data[0] = randRange(255);
    can_frame.data[1] = randRange(255);
    can_frame.data[2] = randRange(255);
    can_frame.data[3] = randRange(255);
    can_frame.data[4] = randRange(255);
    can_frame.data[5] = randRange(255);
    can_frame.data[6] = randRange(255);
    can_frame.data[7] = randRange(255);
    return can_frame;
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

