
#include "stdafx.h"
#include "canal_client.h"
#include "gopt.h"
#include "common.h"
#include <Windows.h>
#include <SYS\STAT.H>

#define LOOPBACK_FLAG  0x00000001
#define SILENT_FLAG    0x00000002
#define DISABLEAR_FLAG 0x00000004 
#define STATUS_FLAG    0x00000008

int threads_started = 0;

struct node *head = NULL;
struct node *current = NULL;
uint32_t current_interface;
int baudrate;
uint32_t flags = 0;
int onCapture;
char *fifo = NULL;
char *ctrlin = NULL;
char* ctrlout = NULL;
int err;
HANDLE readCtrHandle,writeCtrHandle;

bool filecheck(const char* filename)
{
	struct stat info;
	int ret = -1;

	ret = stat(filename, &info);
	return 0 == ret;
}

void MonitorAndCloseCan(uint32_t caninterface, int baudrate)
{
	int error=1;
	struct CanalApi CanalApi = initCanApi();
	HANDLE monpipe = connect_to_pipe("\\\\.\\pipe\\can_extcap_status_pipe");
	char buffer;
	int error_counter = 0;
	while (error_counter < 2) {
		error = PeekNamedPipe(monpipe, &buffer, 1, NULL, NULL, NULL);
		Sleep(500);
		if (0 == error) error_counter++;
	}
	char canstring[16];
	set_interface_string(caninterface, baudrate, canstring);
	uint32_t canhandle = CanalApi.CanalOpen(canstring, flags);
	error=CanalApi.CanalClose(canhandle);

}

void CtrlReadThread(void* data)
{
	readCtrHandle = connect_to_pipe(ctrlin);
	writeCtrHandle = connect_to_pipe(ctrlout);
	while (TRUE) {
		readCtrPipe(readCtrHandle);
	}
}

int main(int argc, char **argv)
{
	/* Extcap interface. */

	struct option options[17];

	options[0].long_name = "extcap-interfaces";
	options[0].short_name = '1';
	options[0].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[1].long_name = "extcap-interface";
	options[1].short_name = '2';
	options[1].flags = GOPT_ARGUMENT_REQUIRED;

	options[2].long_name = "extcap-dlts";
	options[2].short_name = '3';
	options[2].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[3].long_name = "extcap-config";
	options[3].short_name = '4';
	options[3].flags = GOPT_ARGUMENT_OPTIONAL;

	options[4].long_name = "capture";
	options[4].short_name = '5';
	options[4].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[5].long_name = "fifo";
	options[5].short_name = '6';
	options[5].flags = GOPT_ARGUMENT_REQUIRED;

	options[6].long_name = "extcap-version";
	options[6].short_name = '7';
	options[6].flags = GOPT_ARGUMENT_REQUIRED;

	options[7].long_name = "extcap-capture-filter";
	options[7].short_name = '8';
	options[7].flags = GOPT_ARGUMENT_OPTIONAL;

	options[8].long_name = "baudrate";
	options[8].short_name = '9';
	options[8].flags = GOPT_ARGUMENT_REQUIRED;

	options[9].long_name = "extcap-control-out";
	options[9].short_name = 'A';
	options[9].flags = GOPT_ARGUMENT_REQUIRED;

	options[10].long_name = "extcap-control-in";
	options[10].short_name = 'B';
	options[10].flags = GOPT_ARGUMENT_REQUIRED;

	options[11].long_name = "canmonitormode";
	options[11].short_name = 'C';
	options[11].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[12].long_name = "loopback";
	options[12].short_name = 'D';
	options[12].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[13].long_name = "silent";
	options[13].short_name = 'E';
	options[13].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[14].long_name = "disableretransmit";
	options[14].short_name = 'F';
	options[14].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[15].long_name = "status";
	options[15].short_name = 'G';
	options[15].flags = GOPT_ARGUMENT_FORBIDDEN;

	options[16].flags = GOPT_LAST;

	if (!filecheck("extcap\\usb2can.dll")) {
		if (!filecheck("usb2can.dll")) {
			printf("usb2can.dll is missing from the folder. The program will exit now.\n");
			return 0;
		}
	}

	argc = gopt(argv, options);
	gopt_errors(argv[0], options);
	get_can_interfaces();

	if (options[0].count) {
		if (options[6].count) {
			printf("extcap {version=1.0}{help=www.8devices.com}\n");
		}
		struct node *ptr = head;
			while (ptr != NULL) {
				printf("interface {value=%X}{display=USB2CAN interface - s/n %X}\n", (unsigned int)ptr->data, (unsigned int)ptr->data);
				ptr = ptr->next;
			}
		printf("control {number=0}{type=string}{display=Msg ID}{tooltip=Custom frame message ID (0xNNNNNNNN)}{validation=^(([01][a-fA-F0-9]\{0,7\})|([a-fA-F0-9]\{0,7\}))$}\n");
		printf("control {number=1}{type=boolean}{display=Extended}{tooltip=Extended CAN frame}{default=true}\n");
		printf("control {number=2}{type=boolean}{display=RTR}{tooltip=Request to Re-transmit}{default=false}\n");
		printf("control {number=3}{type=string}{display=Message}{tooltip=Custom message (0-8 bytes, space separated)}{validation=^([a-fA-F0-9]\{0,2\}(\\s)*)\{0,8\}$}\n");
		printf("control {number=4}{type=button}{display=Send}{tooltip=Send custom frame if values provided. If empty, sends a random frame}\n");
		//printf("control {number=5}{type=button}{display=Status}{tooltip=Get CAN-BUS status\n");
		//printf("control {number=6}{type=button}{display=Statistics}{tooltip=Get CAN-BUS statistics}\n");
			return 0;
	}
	

	if (options[12].count) {
		flags += LOOPBACK_FLAG;
	}

	if (options[13].count) {
		flags += SILENT_FLAG;
	}

	if (options[14].count) {
		flags += DISABLEAR_FLAG;
	}

	if (options[15].count) {
		flags += STATUS_FLAG;
	}

	if (options[1].count) { 
		int match = 0;
		uint32_t num_arg = (uint32_t)strtoll(options[1].argument, NULL, 16);
		struct node *ptr = head;
		while (ptr != NULL) {
			if (ptr->data == num_arg) {
				match = 1;
				break;
			}
			ptr = ptr->next;
		}
		
		if (match != 1) {
			printf("Invalid interface\n");
			return 0;
		}
		current_interface = num_arg;
	}

	if (options[2].count) {
		printf("dlt {number=113}{name=DLT_LINUX_SLL}{display=USB2CAN}\n");
		return 0;
	}

	if (options[3].count) {
		printf("arg {number=0}{call=--baudrate}{display=Bitrate, kbps}{tooltip=Capture bitrate}{type=integer}{range=10,1000}{required=true}{default=125}\n");
		printf("arg {number=1}{call=--silent}{display=Silent}{tooltip=enable silent mode}{type=boolflag}\n");
		printf("arg {number=2}{call=--loopback}{display=Loopback}{tooltip=enable loopback mode}{type=boolflag}\n");
		printf("arg {number=3}{call=--disableretransmit}{display=Disable auto-retransmissions}{tooltip=disable auto retransmissions}{type=boolflag}\n");
		printf("arg {number=4}{call=--status}{display=Enable status messages}{tooltip=enable status messages}{type=boolflag}\n");
		
	}
	baudrate = 125;

	if (options[8].count) {
		baudrate = (int)strtol(options[8].argument, NULL, 10);
	}

	if (options[11].count) {
		MonitorAndCloseCan(current_interface, baudrate);
		return 0;
	}

	if (options[5].count) {
		fifo = options[5].argument;
	}
	
	if (options[9].count) {
		ctrlout = options[9].argument;
	}

	if (options[10].count) {
		ctrlin = options[10].argument;
	}

	if (options[4].count) {
		onCapture = 1;
		capture_can_pcap(current_interface,flags,baudrate);
	}

	return 0;
}
