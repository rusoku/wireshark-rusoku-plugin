//
// Created by Gediminas Simanskis on 27/12/2024.
//

#ifndef PCAP_HEADERS_H
#define PCAP_HEADERS_H

#include "../include/main.h"

#define LINKTYPE_LINUX_SLL      113
#define DLT_LINUX_SLL           113
#define LINKTYPE_LINUX_SLL2     276
#define DLT_LINUX_SLL2          276
#define LINKTYPE_CAN_SOCKETCAN  409
#define DLT_CAN_SOCKETCAN       409

#define PCAP_PKT_LEN			32
#define SLL_HDR_LEN				16
#define LEN_CAN_FRAME			16	/* total header length */
#define SLL_ADDRLEN				8	/* length of address field */
#define SLL_PROTOCOL_TYPE_CAN	0x000C
#define PKTTYPE_CAN_TX			0x4
#define PKTTYPE_CAN_RX			0x1
#define HATYPE_ARPHRD_NONE      0xFFFE
#define CAN_MAX_DLEN			8
#define CAN_MAX_DLEN			8

// A File Header has the following format.
struct __attribute__((__packed__)) PCAP_FILE_HEADER {
    uint32_t magic;    //0xA1B2C3D4
    uint16_t version_major; //2
    uint16_t version_minor; //4
    uint32_t thiszone; // Reserved1.SHOULD be filled with 0
    uint32_t sigfigs;  // Reserved2.SHOULD be filled with 0
    uint32_t snaplen;
    uint32_t linktype;
};

// A Packet Record is the standard container for storing the packets coming from the network.
struct __attribute__((__packed__)) PCAP_PACKET_RECORD_HEADER{
    uint32_t sec;
    uint32_t usec;
    uint32_t caplen;
    uint32_t len;
};

struct __attribute__((__packed__)) PCAP_LINKTYPE_LINUX_SLL_HEADER {
    uint16_t sll_pkttype;					/* packet type */
    uint16_t sll_hatype;					/* link-layer address type */
    uint16_t sll_halen;						/* link-layer address length */
    uint8_t  sll_addr[SLL_ADDRLEN];			/* link-layer address */
    uint16_t sll_protocol;					/* protocol */
};

struct __attribute__((__packed__)) SOCKETCAN_FRAME_HEADER {
    uint32_t   can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    uint8_t    __pad;   /* padding */
    uint8_t    __res0;  /* reserved / padding */
    uint8_t    __res1;  /* reserved / padding */
    uint8_t    data[CAN_MAX_DLEN];
};

// bendras paketas be failo headerio
//struct pcappkt_can {
//    struct pcap_pkthdr pcap_header;
//    struct sll_header  sll_header;
//    struct can_frame   can_frame;
//};

uint32_t swap_endianness(uint32_t bytes, int bit);
void init_pcap_file_header(struct PCAP_FILE_HEADER *pcap_global_fh);
struct PCAP_LINKTYPE_LINUX_SLL_HEADER init_sll_header(uint16_t pkttype);
struct PCAP_PACKET_RECORD_HEADER init_pcap_pkt_header(void);
struct SOCKETCAN_FRAME_HEADER init_rnd_fake_can_header(void);

#endif //PCAP_HEADERS_H
