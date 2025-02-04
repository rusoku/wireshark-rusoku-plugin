//
// Created by Gediminas Simanskis on 30/12/2024.
//

#include <stdbool.h>
#include <signal.h>
#include "../inc/main.h"
#include "../inc/pcap.h"
#include "../inc/capture_demo.h"
#include "../inc/capture.h"
#include "../inc/comm_base.h"
#include "../inc/pcap_debug.h"
#include "../inc/comm_base.h"
#include "../inc/control_packet.h"

#ifdef _WIN32
#include "windows.h"
static BOOL WINAPI
sighandler(uint64_t dwCtrlType)
#else
static void sighandler(int signo)
#endif
{
    onCapture = 0;
    comm_close_device(0, interface_parameters);
    exit(EXIT_FAILURE);
}

/******************** capture *************************/
void capture(struct INTERFACE_PARAMETERS interface_par) {
    if (interface_par.interface_nr == -1)
        exit(EXIT_FAILURE);

    struct COMM_CAN_MSG can_msg = {};
    FILE *fp_data = NULL;
    FILE *fp_ctrl_out = NULL;
    FILE *fp_ctrl_in = NULL;
    pthread_t control_thread_in, control_thread_out;
    struct PCAP_FILE_HEADER pcap_file_header = {};
    struct PCAP_PACKET_RECORD_HEADER pcap_packet = {};
    struct PCAP_LINKTYPE_CAN_SOCKETCAN pcap_linktype_socketcan = {};

    fp_data = fopen(interface_parameters.fifo_data, "wb");
    if (fp_data == NULL)
        exit(EXIT_FAILURE);

    //ws-to-ext
    fp_ctrl_in = fopen(interface_parameters.fifo_cntrl_in, "rb");
    if (fp_ctrl_in == NULL)
        exit(EXIT_FAILURE);

    //ext-to-ws
    fp_ctrl_out = fopen(interface_parameters.fifo_cntrl_out, "wb");
    if (fp_ctrl_out == NULL)
        exit(EXIT_FAILURE);

    //*** control threads ****
    pthread_create(&control_thread_in, NULL, ctrl_read_thread, (void *) fp_ctrl_in);
    pthread_create(&control_thread_out, NULL, ctrl_send_thread, (void *) fp_ctrl_out);

#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) sighandler, true);
#else
    if (signal(SIGTERM, sighandler) == SIG_ERR)
        exit(EXIT_FAILURE);

    if (signal(SIGINT, sighandler) == SIG_ERR)
        exit(EXIT_FAILURE);
#endif

    if (comm_open_device(0, interface_par) != COMM_SUCCESS) {
        exit(EXIT_FAILURE);
    }

    //CAN_DEBUGPrintf("************** CAN_DEBUG **********************");
    //CAN_DEBUGPrintf("capture:serial_nr=%s\n", interface.serial_str);
    //CAN_DEBUGPrintf("capture:interface_nr=%ld\n", interface.interface_nr);
    //CAN_DEBUGPrintf("capture:dlt_type=%ld\n", interface.dlt_type);
    //CAN_DEBUGPrintf("capture:bitrate=%ld\n", interface.bitrate);
    //CAN_DEBUGPrintf("capture:bitrate_data=%ld\n", interface.bitrate_data);
    //CAN_DEBUGPrintf("capture:options=%08x\n", interface.options);
    //CAN_DEBUGPrintf("\n");

    uint32_t link_type;
    switch (interface_par.dlt_type) {
        case DLT_LINUX_SLL:
            link_type = LINKTYPE_LINUX_SLL;
        break;
        case DLT_LINUX_SLL2:
            link_type = LINKTYPE_LINUX_SLL2;
        break;
        case DLT_CAN_SOCKETCAN:
            link_type = LINKTYPE_CAN_SOCKETCAN;
        break;
        default:
            link_type = LINKTYPE_CAN_SOCKETCAN;
    }

    pcap_prepare_file_header(&pcap_file_header, link_type);
    fwrite(&pcap_file_header, sizeof(struct PCAP_FILE_HEADER), 1, fp_data);
    fflush(fp_data);

    while (onCapture) {
/*
        for (uint32_t x = 0; x <= sizeof(sizeof(can_frame) / sizeof(can_frame[0])); x++) {
            pcap_packet = pcap_prepare_pkt_header(PCAP_SOCKETCAN_PKT_LEN, PCAP_SOCKETCAN_PKT_LEN);
            fwrite(&pcap_packet, sizeof(struct PCAP_PACKET_RECORD_HEADER), 1, fp_data);

            //pcap_linktype_socketcan = init_socketcan_linktype_header();
            pcap_linktype_socketcan = prepare_socketcan_linktype_from_canframe(&can_frame[x]);
            fwrite(&pcap_linktype_socketcan, sizeof(struct PCAP_LINKTYPE_CAN_SOCKETCAN), 1, fp_data);

            fflush(fp_data);
            usleep(10000);
        }
        //while (1)
        //usleep(100000);
*/
    }
    comm_close_device(0, interface_par);
    }
