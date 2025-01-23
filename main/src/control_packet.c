//
// Created by gedsi on 2025-01-22.

#include "../inc/control_packet.h"
#include "../inc/main.h"

/******************** threads ************************/
void *ctrl_read_thread(void *ptr) {
    FILE *ctrl_in;
    ctrl_in = (FILE *) ptr;
    struct CONTROL_PACKET_HEADERS *ctrl_packet_header;
    uint32_t payload_length;
    uint32_t read_len, file_size;;
    uint8_t buff[1024];

    ctrl_packet_header = (struct CONTROL_PACKET_HEADERS *) buff;

    while (onCapture) {
        read_len = fread(buff, 1, 6, ctrl_in);

        DebugPrintf("************** CONTROL PACKET **********************");
        DebugPrintf("control:Sync Pipe Indication=%C\n", ctrl_packet_header->sync);
        DebugPrintf("control:Len2=%d\n", ctrl_packet_header->len[2]);
        DebugPrintf("control:Len1=%d\n", ctrl_packet_header->len[1]);
        DebugPrintf("control:Len0=%d\n", ctrl_packet_header->len[0]);
        DebugPrintf("control:CtlrNumber=%d\n", ctrl_packet_header->ctrl_number);
        DebugPrintf("control:Command=%d\n", ctrl_packet_header->command);

        //payload_length = (ctrl_packet_header->len[1] << 16 | ctrl_packet_header->len[1] << 8 | ctrl_packet_header->len[2]);
        payload_length = ctrl_packet_header->len[2];
        payload_length -= 2;

        if (payload_length != 0) {
            read_len = fread(buff, 1, payload_length, ctrl_in);
            //DebugPrintf("control:data=%d %d %d%", buff[0], buff[1], buff[2], buff[3]);
            DebugPrintf("control:data=%d", buff[0]);
        }
    }

    DebugPrintf("control:RETURN");
    return NULL;
}

void *ctrl_send_thread(void *ptr) {
    FILE *ctrl_out;
    ctrl_out = (FILE *) ptr;



    DebugPrintf("control:RETURN");
    return NULL;
}
