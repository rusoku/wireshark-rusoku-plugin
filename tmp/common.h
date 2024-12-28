#pragma once
#include <tchar.h>
#include <stdint.h>
#include <stdlib.h>

struct node {
	uint32_t	 data;
	struct node *next;
};

extern char payload[64];
extern uint32_t msgid;
extern uint32_t msgflags;
extern int send_can_message;
extern int send_status;
extern int send_statistics;
extern struct node *head;
extern struct node *current;
extern char *fifo,*ctrlin,*ctrlout;
extern uint32_t current_interface;
extern int baudrate;

extern int get_can_interfaces();
extern void insert_interface(uint32_t data);
extern int capture_can_pcap(uint32_t current_interface, uint32_t flags, int baudrate);

extern void CleanupOnExit(void);
extern int onCapture;
extern int threads_started;

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

struct ctrlPkt {
	uint8_t	   sync;
	uint8_t    len[3];
	uint8_t    ctrlNum;
	uint8_t    command;
};

extern uint32_t canal_handle;
extern void CtrlReadThread(void* data);
extern int readCtrPipe(HANDLE hPipe1);
extern int writeCtrThread(char * output_pipe);
extern HANDLE connect_to_pipe(char * pipename);
extern HANDLE open_signal_pipe();