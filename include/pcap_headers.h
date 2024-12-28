//
// Created by Gediminas Simanskis on 27/12/2024.
//

#ifndef PCAP_HEADERS_H
#define PCAP_HEADERS_H

#define LINKTYPE_LINUX_SLL 113

struct __attribute__((__packed__)) _pcap_file_header {
    uint32_t magic;
    unsigned short version_major;
    unsigned short version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t linktype;
};

struct __attribute__((__packed__)) _pcap_packet_header {
    uint32_t sec;
    uint32_t usec;
    uint32_t caplen;
    uint32_t len;
};

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

struct __attribute__((__packed__)) _sll_packet_header {
    uint16_t sll_pkttype;					/* packet type */
    uint16_t sll_hatype;					/* link-layer address type */
    uint16_t sll_halen;						/* link-layer address length */
    uint8_t  sll_addr[SLL_ADDRLEN];			/* link-layer address */
    uint16_t sll_protocol;					/* protocol */
};


#endif //PCAP_HEADERS_H
