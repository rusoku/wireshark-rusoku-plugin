//
// Created by Gediminas Simanskis on 30/12/2024.
//

#include "../include/main.h"
#include "../include/pcap.h"
#include "../include/capture.h"

#include <stdbool.h>

void capture_demo(char *fifo_name, struct INTERFACE_PARAMETERS *interface_parameters, int16_t interface)
{
        if (interface == -1)
                exit (EXIT_FAILURE);

/*
        0.010sec 7E8 10 2D 62 E0 01 9F F7 FE ---> First Frame for Multi Frame. 45 Bytes(2D) datas contained in '62 E0 01' response
        0.020sec 7E8 21 00 00 AA 00 00 00 00 ---> Consecutive Frame for First datas in '62 E0 01' response
        0.030sec 7E8 22 00 00 00 00 00 00 00 ---> Consecutive Frame for Second datas in '62 E0 01' response
        0.040sec 7E8 23 00 00 00 00 00 00 00 ---> Consecutive Frame for Third datas in '62 E0 01' response
        0.050sec 7E8 24 ~~~~~~ ---> Consecutive Frame for Fourth datas in '62 E0 01' response
        ~~~~
        0.080sec 7E8 10 1A 62 E0 04 FF FF 07 ---> First Frame for Multi Frame. 26 Bytes(1A) datas contained in '62 E0 04' response
        0.090sec 7E8 21 00 00 00 00 00 00 00 ---> Consecutive Frame for First datas in '62 E0 04' response
        ~~~~
        0.210sec 7E8 10 2D 62 E0 01 9F F7 FE
        0.220sec 7E8 21 00 00 AE 00 00 00 00
  */

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
                exit (EXIT_FAILURE);
        //setvbuf(fp, NULL, _IONBF, 0);

        init_pcap_file_header(&pcap_file_header, LINKTYPE_CAN_SOCKETCAN);
        fwrite(&pcap_file_header, sizeof(struct PCAP_FILE_HEADER), 1, fp);
        fflush(fp);

        while (1)
        {
                for (uint32_t x = 0; x <= sizeof(sizeof(can_frame)/sizeof(can_frame[0])); x++)
                {
                        pcap_packet = init_pcap_pkt_header(PCAP_SOCKETCAN_PKT_LEN, PCAP_SOCKETCAN_PKT_LEN);
                        fwrite(&pcap_packet, sizeof(struct PCAP_PACKET_RECORD_HEADER), 1, fp);

                        //pcap_linktype_socketcan = init_socketcan_linktype_header();
                        pcap_linktype_socketcan = prepare_socketcan_linktype_from_canframe(&can_frame[x]);
                        fwrite(&pcap_linktype_socketcan, sizeof(struct PCAP_LINKTYPE_CAN_SOCKETCAN), 1, fp);

                        fflush(fp);
                        usleep(1000);
                }
                while (1)
                        usleep(1000);
        }
}
