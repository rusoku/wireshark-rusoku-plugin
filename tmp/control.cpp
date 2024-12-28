#include "stdafx.h"
#include "canal_client.h"
#include "common.h"
#include "canpcap.h"
#include <time.h>
#include <Windows.h>

char payload[64];
uint32_t msgid = 0;
uint32_t msgflags = 1;
int send_can_message = 0;
int send_status = 0;
int send_statistics = 0;

int setCtrlPkt(ctrlPkt *control_packet, uint8_t ctrlNum, uint8_t command)
{
	memset(control_packet, 0, sizeof(control_packet));
	control_packet->sync = 'T';
	control_packet->ctrlNum = ctrlNum;
	control_packet->command = command;
	return 0;
}

HANDLE open_signal_pipe(void) {
	wchar_t * pipename = L"\\\\.\\pipe\\can_extcap_status_pipe";
	HANDLE hPipe;
	while (TRUE) {
		hPipe=CreateNamedPipe(
			pipename,            
			PIPE_ACCESS_DUPLEX |    
			FILE_FLAG_OVERLAPPED,     
			PIPE_TYPE_MESSAGE |      
			PIPE_READMODE_MESSAGE |  
			PIPE_NOWAIT,               
			128,              
			10 * sizeof(TCHAR),   
			10 * sizeof(TCHAR),   
			12000000,            
			NULL);                   

		if (hPipe != INVALID_HANDLE_VALUE) break;
	}
	char monitorprocessstring[64];
	snprintf(monitorprocessstring, sizeof(monitorprocessstring), "usb_can_extcap.exe --canmonitormode --extcap-interface %X --baudrate %d", current_interface, baudrate);

	PROCESS_INFORMATION     pi;
	STARTUPINFOA            si;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	CreateProcessA(
		NULL,
		monitorprocessstring,
		NULL,
		NULL,
		TRUE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&si,
		&pi
	);
	return hPipe;
}

HANDLE connect_to_pipe(char * pipename_char)
{
	wchar_t pipename[256];
	swprintf_s(pipename, 128, L"%hs", pipename_char);
	HANDLE hPipe;
	while (TRUE) {
		hPipe = CreateFile(
			pipename,
			GENERIC_READ |
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (hPipe != INVALID_HANDLE_VALUE) break;
	}
	return hPipe;
}

int writeCtrPipe(HANDLE hpipe1)
{
	return 0;
}



int readCtrPipe(HANDLE hPipe1)
{
	int error;
	DWORD numRead,numWritten,bytesavailable;
	ctrlPkt control_packet;
	memset(&control_packet, 0, sizeof(ctrlPkt));

	char buffer[256];
	memset(&buffer, 0, sizeof(buffer));
	if (hPipe1 == INVALID_HANDLE_VALUE) return -1;
	error = PeekNamedPipe(hPipe1, &buffer, sizeof(buffer), &numRead, &bytesavailable,	NULL);
	int pkt_length = 0;

	if (bytesavailable != 0) {
		error = ReadFile(hPipe1, &control_packet, sizeof(ctrlPkt), &numWritten, NULL);
		pkt_length = control_packet.len[2] + (control_packet.len[1] << 8) + (control_packet.len[0] << 16);
		if (pkt_length > 2) {
			error = ReadFile(hPipe1, &buffer, pkt_length-2, &numWritten, NULL);
			buffer[pkt_length-2] = '\0';
		}
	}

//	struct __attribute__((__packed__)) _control_packet_headers {
//		uint8_t	   sync;
//		uint8_t    len[3];
//		uint8_t    ctrlNum;
//		uint8_t    command;
//	};

	if (control_packet.sync == 'T') {
		switch (control_packet.ctrlNum) {
		case 0:
			if (control_packet.command == 1) msgid = strtol(buffer,NULL,16);
		break;
		case 1:
			if (control_packet.command == 1) {
				if (buffer[0])
                	msgflags |= 1;
				else
                    msgflags &= ~1;
			}
		break;
		case 2:
			if (control_packet.command == 1) {
				if (buffer[0])
                	msgflags |= 2;
				else
                    msgflags &= ~2;
			}
		break;
		case 3:
			memset(payload, 0, sizeof(payload));
			if (control_packet.command == 1) {
				for (int i = 0; i < pkt_length - 1; i++) {
					payload[i] = buffer[i];
				}
			}
		break;
		case 4:
			if (control_packet.command == 1) send_can_message += 1;
		break;
		case 5:
			send_status = 1;
			//TODO
			break;
		case 6:
			send_statistics = 1;
			//TODO
			break;
		}
		}
	/*	printf("control {number=0}{type=string}{display=Msg ID}{tooltip=Custom frame message ID (0xNNNNNNNN)}{validation=^[01]([a-fA-f0-9]\\{0,7\\})$}\n");
		printf("control {number=1}{type=boolean}{display=Extended}{tooltip=Extended CAN frame}{default=true}\n");
		printf("control {number=2}{type=boolean}{display=RTR}{tooltip=Request to Re-transmit}{default=false}\n");
		printf("control {number=3}{type=string}{display=Message}{tooltip=Custom message (0-8 bytes, space separated)}{validation=(^[a-fA-f0-9 ]\\{0,23\\})$}\n");
		printf("control {number=4}{type=button}{display=Send}{tooltip=Send custom frame if values provided. If empty, sends a random frame}\n");
		printf("control {number=5}{type=button}{display=Status}{tooltip=Get CAN-BUS status\n");
		printf("control {number=6}{type=button}{display=Statistics}{tooltip=Get CAN-BUS statistics}\n");*/
	return 0;
}

int writeCtrThread(char *output_pipe)
{
	return 0;
}