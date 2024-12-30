//
// Created by Gediminas Simanskis on 30/12/2024.
//

#include "../include/main.h"
#include "../include/pcap.h"
#include "../include/capture.h"

void capture(char *fifo_name, struct INTERFACE_PARAMETERS *interface_parameters)
{
        FILE *fp;
        struct PCAP_FILE_HEADER pcap_file_header = {};
        struct PCAP_PACKET_RECORD_HEADER pcap_packet = {};
        struct PCAP_LINKTYPE_LINUX_SLL_HEADER pcap_linktype = {};
        struct SOCKETCAN_FRAME_HEADER can_frame = {};

        fp = fopen(fifo_name, "wb");
        if (fp == NULL)
                exit (EXIT_FAILURE);

        init_pcap_file_header(&pcap_file_header);
        fwrite(&pcap_file_header, sizeof(struct PCAP_FILE_HEADER), 1, fp);

        while (1)
        {
                pcap_packet = init_pcap_pkt_header();
                fwrite(&pcap_packet, sizeof(struct PCAP_PACKET_RECORD_HEADER), 1, fp);

                pcap_linktype = init_sll_header(PKTTYPE_CAN_TX);
                fwrite(&pcap_linktype, sizeof(struct PCAP_LINKTYPE_LINUX_SLL_HEADER), 1, fp);

                can_frame = init_rnd_fake_can_header();
                fwrite(&can_frame, sizeof(struct SOCKETCAN_FRAME_HEADER), 1, fp);

                usleep(5000);
        }
}