//
// Created by Gediminas Simanskis on 27/12/2024.
//

#ifndef CONTROL_PACKET_HEADERS_H
#define CONTROL_PACKET_HEADERS_H

#define CTRL_CMD_INITIALIZED 0
#define CTRL_CMD_SET		 1
#define CTRL_CMD_ADD		 2
#define CTRL_CMD_REMOVE		 3
#define CTRL_CMD_ENABLE		 4
#define CTRL_CMD_DISABLE	 5
#define CTRL_CMD_STATUSBAR	 6
#define CTRL_CMD_INFORMATION 7
#define CTRL_CMD_WARNING	 8
#define CTRL_CMD_ERROR		 9

struct __attribute__((__packed__)) _control_packet_headers {
    uint8_t	   sync;
    uint8_t    len[3];
    uint8_t    ctrlNum;
    uint8_t    command;
};

#endif //CONTROL_PACKET_HEADERS_H
