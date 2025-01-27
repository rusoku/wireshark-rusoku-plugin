//
// Created by gedsi on 2025-01-22.

#include "../inc/control_packet.h"
#include "../inc/comm_base.h"
#include "../inc/main.h"
#include "../inc/pcap_debug.h"

/******************** threads ************************/
void *ctrl_read_thread(void *ptr) {
    FILE *ctrl_in;
    ctrl_in = (FILE *) ptr;
    uint32_t payload_length;
    struct CONTROL_PACKET_HEADERS ctrl_packet = {0};
    uint8_t buff[2048];
    int64_t pos;
    struct COMM_CAN_MSG can_msg = {};

    while (onCapture) {
        fread(&ctrl_packet, sizeof(struct CONTROL_PACKET_HEADERS), 1, ctrl_in);
        /*
                PCAP_DEBUG("************** CONTROL PACKET **********************");
                PCAP_DEBUG("control:Sync Pipe Indication=%C\n", ctrl_packet.sync);
                PCAP_DEBUG("control:Len2=%d\n", ctrl_packet.len[2]);
                PCAP_DEBUG("control:Len1=%d\n", ctrl_packet.len[1]);
                PCAP_DEBUG("control:Len0=%d\n", ctrl_packet.len[0]);
                PCAP_DEBUG("control:CtlrNumber=%d\n", ctrl_packet.ctrl_number);
                PCAP_DEBUG("control:Command=%d\n", ctrl_packet.command);
        */
        payload_length = (ctrl_packet.len[1] << 16 | ctrl_packet.len[1] << 8 | ctrl_packet.len[2]);

        if (ctrl_packet.sync != 'T') {
            fseek(ctrl_in, 0, SEEK_SET);
            pos = ftell(ctrl_in);
            if (pos > 0) {
                fread(buff, 1, (pos & sizeof(buff)), ctrl_in);
            }
        }

        payload_length = ctrl_packet.len[2];
        payload_length -= 2;
        /*
                printf(
                    "control {number=0}{type=string}{display=Msg ID}{tooltip=Custom frame message ID (0xNNNNNNNN)}{validation=^(([01][a-fA-F0-9]{0,7})|([a-fA-F0-9]{0,7}))$}\n");
                printf("control {number=1}{type=boolean}{display=Extended}{tooltip=Extended CAN frame}{default=true}\n");
                printf(
                    "control {number=2}{type=string}{display=Message}{tooltip=Custom message (0-8 bytes, space separated)}{validation=^([a-fA-F0-9]{0,2}(s)*){0,8}$}\n");
                printf(
                    "control {number=3}{type=button}{display=Send}{tooltip=Send custom frame if values provided. If empty, sends a random frame}\n");
          */
        /*
                struct COMM_CAN_MSG {
                    uint32_t id;
                    uint32_t error_status; // error counters (if CAN_FLAG_STATUS is set)
                    uint32_t error_code; // error codes
                    uint32_t flags; // can flags: STD, EXT, CAN FD etc.
                    uint8_t length;
                    uint8_t data[64];
                };
        */
        if (ctrl_packet.command == CTRL_CMD_SET) {
            switch (ctrl_packet.ctrl_number) {
                case CONTROL_0: //CAN id
                    if (payload_length != 0) {
                        fread(buff, 1, payload_length, ctrl_in);
                        buff[payload_length] = 0;
                        can_msg.id = strtol((char *) buff, NULL, 16);
                    }
                    break;
                case CONTROL_1: //Extended CAN frame
                    if (payload_length != 0) {
                        fread(buff, 1, payload_length, ctrl_in);
                        if (buff[0] == 1) {
                            can_msg.flags |= CAN_FLAG_EXTENDED;
                        } else {
                            can_msg.flags &= ~CAN_FLAG_EXTENDED;
                        }
                    }
                    break;
                case CONTROL_2: // CAN data
                    if (payload_length != 0) {
                        fread(buff, 1, payload_length, ctrl_in);
                        buff[payload_length] = 0;
                        can_msg.data[0] = strtol((char *) buff, NULL, 16);
                    }
                    break;
                case CONTROL_3: // SEND button
                    PCAP_DEBUG("CAN id=%08X,CAN flags=%X, CAN data0=%02X)", can_msg.id, can_msg.flags, can_msg.data[0]);
                //memset(&can_msg, 0, sizeof(can_msg));
                    break;
                default:
                    break;
            }
        }
    }

    PCAP_DEBUG("control:RETURN");
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

    CAN_DEBUG("control:RETURN");
    */
    return NULL;
}
