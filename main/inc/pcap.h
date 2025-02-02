//
// Created by Gediminas Simanskis on 27/12/2024.
//

#ifndef PCAP_H
#define PCAP_H

#include "main.h"

#define LINKTYPE_LINUX_SLL      113
#define DLT_LINUX_SLL           0
#define LINKTYPE_LINUX_SLL2     276
#define DLT_LINUX_SLL2          1
#define LINKTYPE_CAN_SOCKETCAN  227
#define DLT_CAN_SOCKETCAN       2

#define PCAP_SLL_PKT_LEN		32
#define PCAP_SOCKETCAN_PKT_LEN  16
#define SLL_HDR_LEN				16
#define LEN_CAN_FRAME			16	/* total header length */
#define SLL_ADDRLEN				8	/* length of address field */
#define SLL_PROTOCOL_TYPE_CAN	0x000C
#define PKTTYPE_CAN_TX			0x4
#define PKTTYPE_CAN_RX			0x1
#define HATYPE_ARPHRD_NONE      0xFFFE
#define CAN_MAX_DLEN			8

#ifdef __GNUC__
#define PACK__
#define __PACK __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#define PACK__ __pragma( pack(push, 1) )
#define __PACK __pragma( pack(pop))
#endif

// A File Header has the following format.
PACK__ struct PCAP_FILE_HEADER {
    uint32_t    magic;    //0xA1B2C3D4
    uint16_t    version_major; //2
    uint16_t    version_minor; //4
    uint32_t    thiszone; // Reserved1.SHOULD be filled with 0
    uint32_t    sigfigs;  // Reserved2.SHOULD be filled with 0
    uint32_t    snaplen;
    uint32_t    linktype;
}__PACK;

/**************** LINKTYPE_LINUX_SLL 	113 	DLT_LINUX_SLL ****************/
// A Packet Record is the standard container for storing the packets coming from the network.
PACK__ struct PCAP_PACKET_RECORD_HEADER{
    uint32_t    sec;
    uint32_t    usec;
    uint32_t    caplen;
    uint32_t    len;
}__PACK;

//https://www.tcpdump.org/linktypes/LINKTYPE_LINUX_SLL.html
PACK__ struct PCAP_LINKTYPE_LINUX_SLL_HEADER {
    uint16_t    sll_pkttype;					/* packet type */
    uint16_t    sll_hatype;				        /* link-layer address type */
    uint16_t    sll_halen;						/* link-layer address length */
    uint8_t     sll_addr[SLL_ADDRLEN];			/* link-layer address */
    uint16_t    sll_protocol;					/* protocol */
}__PACK;

/********************* LINKTYPE_IPV4 	228 	DLT_IPV4 *******************************/
//https://www.tcpdump.org/linktypes/LINKTYPE_CAN_SOCKETCAN.html
PACK__ struct PCAP_LINKTYPE_CAN_SOCKETCAN {
    uint32_t    can_id;			        		/* can id */
    uint8_t     payload_length;;                /* payload length */
    uint8_t     fd_flags;						/* fd lags */
    uint8_t     reserved1;			            /* reserved1 */
    uint8_t     reserved2;  					/* reserved2 */
    uint8_t     data[CAN_MAX_DLEN];
}__PACK;

PACK__ struct SOCKETCAN_FRAME_HEADER {
    uint32_t   can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    uint8_t    __pad;   /* padding */
    uint8_t    __res0;  /* reserved / padding */
    uint8_t    __res1;  /* reserved / padding */
    uint8_t    data[CAN_MAX_DLEN];
}__PACK;

/*********** LINKTYPE_LINUX_SLL2 	276 	DLT_LINUX_SLL2 *******************/
//https://www.tcpdump.org/linktypes/LINKTYPE_LINUX_SLL2.html
PACK__ struct PCAP_LINKTYPE_LINUX_SLL2_HEADER {
    uint16_t    sll2_protocol_type;					/* packet type */
    uint16_t    sll2_hatype;				        /* link-layer address type */
    uint16_t    sll2_halen;						/* link-layer address length */
    uint8_t     sll2_addr[SLL_ADDRLEN];			/* link-layer address */
    uint16_t    sll2_protocol;					/* protocol */
}__PACK;



struct CAN_FRAME {
    uint32_t   can_id;
    uint8_t    can_ext;
    uint8_t    can_dlc;
    uint8_t    can_data[8];
};


uint32_t swap_endianness(uint32_t bytes, int bit);
void pcap_prepare_file_header(struct PCAP_FILE_HEADER *pcap_global_fh, uint32_t dlt);
struct PCAP_LINKTYPE_LINUX_SLL_HEADER pcap_prepare_sll_header(uint16_t pkttype);
struct PCAP_LINKTYPE_CAN_SOCKETCAN pacap_prepare_socketcan_linktype_header(void);
struct PCAP_LINKTYPE_LINUX_SLL_HEADER pcap_prepare_sll2_header(uint16_t pkttype);

struct PCAP_PACKET_RECORD_HEADER pcap_prepare_pkt_header(uint32_t pkt_cap_len, uint32_t pkt_len);
struct SOCKETCAN_FRAME_HEADER init_rnd_fake_can_header(void);
struct PCAP_LINKTYPE_CAN_SOCKETCAN prepare_socketcan_linktype_from_canframe(struct CAN_FRAME *can_frame);

#endif //PCAP_HEADERS_H
