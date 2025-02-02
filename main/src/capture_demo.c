//
// Created by Gediminas Simanskis on 30/12/2024.
//

#include "../inc/main.h"
#include "../inc/pcap.h"
#include "../inc/capture_demo.h"

void capture_demo(char *fifo_name, struct INTERFACE_PARAMETERS interface_parameters) {
    if (interface_parameters.interface_nr == -1)
        exit(EXIT_FAILURE);


    struct CAN_FRAME can_frame[] = {
        {0x7E8, 0, 8, 0x10, 0x2D, 0x62, 0xE0, 0x01, 0x9F, 0xF7, 0xFE},
        {0x7E8, 0, 8, 0x21, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00},
        {0x7E8, 0, 8, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x7E8, 0, 8, 0x23, 0, 0, 0, 0, 0, 0, 0},
        {0x7E8, 0, 8, 0x24, 0, 0, 0, 0, 0, 0, 0},
        {0x7E8, 0, 8, 0x10, 0x1A, 0x62, 0xE0, 0x04, 0xFF, 0xFF, 0x07},
        {0x7E8, 0, 8, 0x21, 0, 0, 0, 0, 0, 0, 0},
        {0x7E8, 0, 8, 0x10, 0x2D, 0x62, 0xE0, 0x01, 0x9F, 0xF7, 0xFE},
        {0x7E8, 0, 8, 0x21, 0, 0, 0xAE, 0, 0, 0, 0}
    };

    FILE *fp;
    struct PCAP_FILE_HEADER pcap_file_header = {};
    struct PCAP_PACKET_RECORD_HEADER pcap_packet = {};
    struct PCAP_LINKTYPE_CAN_SOCKETCAN pcap_linktype_socketcan = {};

    fp = fopen(fifo_name, "wb");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    //setvbuf(fp, NULL, _IONBF, 0);

    pcap_prepare_file_header(&pcap_file_header, LINKTYPE_CAN_SOCKETCAN);
    fwrite(&pcap_file_header, sizeof(struct PCAP_FILE_HEADER), 1, fp);
    fflush(fp);

    while (1) {
        for (uint32_t x = 0; x <= sizeof(sizeof(can_frame) / sizeof(can_frame[0])); x++) {
            pcap_packet = pcap_prepare_pkt_header(PCAP_SOCKETCAN_PKT_LEN, PCAP_SOCKETCAN_PKT_LEN);
            fwrite(&pcap_packet, sizeof(struct PCAP_PACKET_RECORD_HEADER), 1, fp);
            pcap_linktype_socketcan = prepare_socketcan_linktype_from_canframe(&can_frame[x]);
            fwrite(&pcap_linktype_socketcan, sizeof(struct PCAP_LINKTYPE_CAN_SOCKETCAN), 1, fp);
            fflush(fp);

            usleep(1000);
        }
        while (1)
            usleep(1000);
    }
}
