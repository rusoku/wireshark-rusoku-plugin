#include <stdlib.h>
#include <tchar.h>
#include <stdint.h>
#include <winsock.h>


struct pcap_file_header {
	uint32_t magic;
	unsigned short version_major;
	unsigned short version_minor;
	int32_t thiszone;
	uint32_t sigfigs;  
	uint32_t snaplen;  
	uint32_t linktype;
};

#define LINKTYPE_LINUX_SLL 113

struct pcap_pkthdr {
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

struct sll_header {
	uint16_t sll_pkttype;					/* packet type */
	uint16_t sll_hatype;					/* link-layer address type */
	uint16_t sll_halen;						/* link-layer address length */
	uint8_t  sll_addr[SLL_ADDRLEN];			/* link-layer address */
	uint16_t sll_protocol;					/* protocol */
};

/*The packet type field is in network byte order(big - endian); it contains a value that is one of :

0, if the packet was specifically sent to us by somebody else;
1, if the packet was broadcast by somebody else;
2, if the packet was multicast, but not broadcast, by somebody else;
3, if the packet was sent to somebody else by somebody else;
4, if the packet was sent by us.
*/

struct can_frame {
	uint32_t   can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	uint8_t    __pad;   /* padding */
	uint8_t    __res0;  /* reserved / padding */
	uint8_t    __res1;  /* reserved / padding */
	uint8_t    data[CAN_MAX_DLEN];
};

struct pcappkt_can {
	struct pcap_pkthdr pcap_header;
	struct sll_header  sll_header;
	struct can_frame   can_frame;
};
