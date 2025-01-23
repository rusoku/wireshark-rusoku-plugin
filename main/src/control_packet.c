//
// Created by gedsi on 2025-01-22.

#include "../inc/control_packet.h"
#include "../inc/main.h"

/******************** threads ************************/
void *ctrl_read_thread(void *ptr) {
    FILE *ctrl_in;
    ctrl_in = (FILE *) ptr;
    uint32_t payload_length;
    uint32_t read_len, file_size;;
    struct CONTROL_PACKET_HEADERS ctrl_packet = {0};
    uint8_t buff[512];

    while (onCapture) {
        fread(&ctrl_packet, sizeof(struct CONTROL_PACKET_HEADERS), 1, ctrl_in);

        DebugPrintf("************** CONTROL PACKET **********************");
        DebugPrintf("control:Sync Pipe Indication=%C\n", ctrl_packet.sync);
        DebugPrintf("control:Len2=%d\n", ctrl_packet.len[2]);
        DebugPrintf("control:Len1=%d\n", ctrl_packet.len[1]);
        DebugPrintf("control:Len0=%d\n", ctrl_packet.len[0]);
        DebugPrintf("control:CtlrNumber=%d\n", ctrl_packet.ctrl_number);
        DebugPrintf("control:Command=%d\n", ctrl_packet.command);

        //payload_length = (ctrl_packet_header->len[1] << 16 | ctrl_packet_header->len[1] << 8 | ctrl_packet_header->len[2]);
        payload_length = ctrl_packet.len[2];
        payload_length -= 2;

        switch (ctrl_packet.ctrl_number) {
            case 1:

                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            default:
        }

        if (payload_length != 0) {
            read_len = fread(buff, 1, payload_length, ctrl_in);
        }
    }

    DebugPrintf("control:RETURN");
    return NULL;
}

void *ctrl_send_thread(void *ptr) {
    FILE *ctrl_out;
    ctrl_out = (FILE *) ptr;
    //const uint8_t buff[] = "123456789";

    /*
    struct CONTROL_PACKET_HEADERS ctrl_packet = {0};
    ctrl_packet.sync = 'T';
    ctrl_packet.len[2] = 10;
    ctrl_packet.ctrl_number = 0;
    ctrl_packet.command = 6;

    fwrite(&ctrl_packet, sizeof(struct CONTROL_PACKET_HEADERS), 1, ctrl_out);
    fwrite(&ctrl_packet, 10, 1, ctrl_out);

    DebugPrintf("control:RETURN");
    */
    return NULL;
}
