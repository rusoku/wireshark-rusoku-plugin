//
// Created by Gediminas Simanskis on 30/12/2024.
//

#include "../include/main.h"
#include "../include/pcap.h"
#include "../include/capture.h"

void capture(char *fifo_name, struct INTERFACE_PARAMETERS *interface_parameters, int16_t interface)
{
        //if (interface == -1)
        //        exit (EXIT_FAILURE);

        FILE *fp;
        struct PCAP_FILE_HEADER pcap_file_header = {};
        struct PCAP_PACKET_RECORD_HEADER pcap_packet = {};
        struct PCAP_LINKTYPE_CAN_SOCKETCAN pcap_linktype_socketcan = {};

        fp = fopen(fifo_name, "wb");
        if (fp == NULL)
                exit (EXIT_FAILURE);

        init_pcap_file_header(&pcap_file_header, LINKTYPE_CAN_SOCKETCAN);
        fwrite(&pcap_file_header, sizeof(struct PCAP_FILE_HEADER), 1, fp);

        while (1)
        {
                for (uint32_t x = 0; x < 128; x++)
                {
                        pcap_packet = init_pcap_pkt_header(PCAP_SOCKETCAN_PKT_LEN, PCAP_SOCKETCAN_PKT_LEN);
                        fwrite(&pcap_packet, sizeof(struct PCAP_PACKET_RECORD_HEADER), 1, fp);

                        pcap_linktype_socketcan = init_socketcan_linktype_header();
                        fwrite(&pcap_linktype_socketcan, sizeof(struct PCAP_LINKTYPE_CAN_SOCKETCAN), 1, fp);
                }

                while (1)
                        usleep(1000);
        }
}