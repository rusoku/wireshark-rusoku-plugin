//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2025 Gediminas Simanskis, Rusoku technologijos UAB, Vilnius, Lithuania (info@rusoku.com)
//  All rights reserved.
//
//  GNU General Public License v3.0 or later:
//  Rusoku wireshark CAN bus adapters extcap plugin is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Rusoku wireshark extcap plugin is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Rusoku wireshark extcap plugin.If not, see <https://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../inc/pcap_param.h"
#include "../inc/control_packet.h"
#include "../inc/comm_base.h"
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
                        payload_length -= 2; // remove "0x" from string
                        uint8_t tmp_buff[3] = {};
                        uint32_t buffer_index = 2, data_index = 0;
                        while (buffer_index <= payload_length) {
                            memcpy(&tmp_buff[0], &buff[buffer_index++], 1);
                            memcpy(&tmp_buff[1], &buff[buffer_index++], 1);
                            tmp_buff[2] = 0;

                            if (data_index > 7)
                                continue;
                            can_msg.data[data_index++] = strtol((char *) tmp_buff, NULL, 16);
                            can_msg.length = data_index;
                        }
                    }
                    break;
                case CONTROL_3: // SEND button
                    comm_write_frame(0, &can_msg);
                    break;
                default:
                    break;
            }
        }
    }

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
    usleep(1000);
    return NULL;
}
