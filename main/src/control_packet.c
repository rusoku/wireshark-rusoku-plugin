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

    uint8_t buff_ctrl_header[6];
    uint8_t buff_ctrl_payload[1024];
    ctrl_packet_header = (struct CONTROL_PACKET_HEADERS *) buff_ctrl_header;

    while (onCapture) {
        fread(buff_ctrl_header, 1, sizeof(buff_ctrl_header), ctrl_in);

        DebugPrintf("************** CONTROL PACKET **********************");
        DebugPrintf("control:Sync Pipe Indication=%C\n", ctrl_packet_header->sync);
        DebugPrintf("control:Len2=%d\n", ctrl_packet_header->len[2]);
        DebugPrintf("control:Len1=%d\n", ctrl_packet_header->len[1]);
        DebugPrintf("control:Len0=%d\n", ctrl_packet_header->len[0]);
        DebugPrintf("control:CtlrNumber=%d\n", ctrl_packet_header->ctrl_number);
        DebugPrintf("control:Command=%d\n", ctrl_packet_header->command);

        payload_length = (((ctrl_packet_header->len[2]) | (ctrl_packet_header->len[1] << 8) | (
                              ctrl_packet_header->len[0] << 16)) - 2);

        DebugPrintf("control:PAYLOAD_LENGTH=%d\n", payload_length);
    }
    DebugPrintf("control:RETURN");
    return NULL;
}
