//
// Created by gedsi on 2025-01-22.
//

#ifndef CONTROL_PACKET_H
#define CONTROL_PACKET_H

#include <stdint.h>

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

#define CONTROL_0  0
#define CONTROL_1  1
#define CONTROL_2  2
#define CONTROL_3  3
#define CONTROL_4  4
#define CONTROL_5  5
#define CONTROL_6  6
#define CONTROL_7  7
#define CONTROL_8  8
#define CONTROL_9  9
#define CONTROL_10 10

#ifdef __GNUC__
#define PACK__
#define __PACK __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#define PACK__ __pragma( pack(push, 1) )
#define __PACK __pragma( pack(pop))
#endif

PACK__ struct CONTROL_PACKET_HEADERS {
    uint8_t	   sync;
    uint8_t    len[3];
    uint8_t    ctrl_number;
    uint8_t    command;
}__PACK;;

void *ctrl_read_thread(void *ptr);
void *ctrl_send_thread(void *ptr);

#endif //CONTROL_PACKET_H
