#include "stdafx.h"
#include "canal_client.h"
#include "common.h"
#include "canpcap.h"
#include <time.h>
#include <Windows.h>
#include <winsock.h>


#define BITWISE 1
#define BYTE_ORDER 0
#define TEST_CODE
#define CANAL_FLAG_XTD 1


uint32_t canal_handle;

uint32_t swap_endianness(uint32_t bytes, int bit = BYTE_ORDER) {
	bytes = (bytes & 0xFFFF0000) >> 16 | (bytes & 0x0000FFFF) << 16;
	bytes = (bytes & 0xFF00FF00) >> 8  | (bytes & 0x00FF00FF) << 8;
	if (bit == 1) {
		bytes = (bytes & 0xF0F0F0F0) >> 4  | (bytes & 0x0F0F0F0F) << 4;
		bytes = (bytes & 0xCCCCCCCC) >> 2 | (bytes & 0x33333333) << 2;
		bytes = (bytes & 0xAAAAAAAA) >> 1 | (bytes & 0x55555555) << 1;
	}
	else if (bit == 16) bytes = bytes >> 16;
	return bytes;
}

int convert_canal_to_socketcan(can_frame* can_frame, structCanalMsg* canal_frame)
{
	memset(can_frame, 0, LEN_CAN_FRAME);
	can_frame->can_id = swap_endianness((uint16_t)canal_frame->flags,1) | canal_frame->id;
	can_frame->can_dlc = canal_frame->sizeData;
	memcpy(can_frame->data, canal_frame->data, CAN_MAX_DLEN);
	return 0;
}


void get_timestamp(uint32_t *seconds, uint32_t *msec)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));  
	SYSTEMTIME timestamp;
	LPSYSTEMTIME pTime = &timestamp;
	GetSystemTime(pTime);
	tm.tm_year = pTime->wYear - 1900;
	tm.tm_mon = pTime->wMonth - 1;
	tm.tm_mday = pTime->wDay;

	tm.tm_hour = pTime->wHour;
	tm.tm_min = pTime->wMinute;
	tm.tm_sec = pTime->wSecond;
	tm.tm_isdst = -1;
	*seconds = uint32_t(mktime(&tm));
	*msec = uint32_t(pTime->wMilliseconds)*1000;
}

pcap_pkthdr init_pcap_pkt_header(structCanalMsg *canal_frame, pcappkt_can *pcap_frame)
{
	pcap_pkthdr pcap_packet_header;
	
	get_timestamp(&pcap_packet_header.sec, &pcap_packet_header.usec);
	pcap_packet_header.caplen = PCAP_PKT_LEN;
	pcap_packet_header.len = PCAP_PKT_LEN;
	return pcap_packet_header;
}

sll_header init_sll_header(uint16_t pkttype) 
{
	sll_header sll_header;
	sll_header.sll_pkttype = swap_endianness(pkttype, 16);
	sll_header.sll_hatype = swap_endianness(HATYPE_ARPHRD_NONE, 16);
	sll_header.sll_halen = 0;
	for (int i = 0; i < SLL_ADDRLEN; i++) sll_header.sll_addr[i] = 0xFF;
	sll_header.sll_protocol = swap_endianness(SLL_PROTOCOL_TYPE_CAN, 16);
	return sll_header;
}

structCanalMsg random_canal_frame() {
	structCanalMsg random_canal;
	random_canal.id = 0x0;
	srand((uint32_t)time(NULL));
	uint32_t rand_err_flag = 0;
	uint32_t rand_xtd_flag = rand_err_flag ? 0 : !!(rand() % 5);
	uint32_t rand_tx_flag  = swap_endianness(1,1);

	random_canal.flags = rand_tx_flag | (rand_err_flag << 2) | rand_xtd_flag;
	if (rand_xtd_flag) random_canal.id = ((uint32_t)rand() << 16 | rand()) & 0x1FFFFFFF;
	else random_canal.id = (rand_err_flag ? 0x0 : (rand() & 0x000007FF));
	random_canal.sizeData = rand() % 8;
	
	for (uint8_t i = 0; i < CAN_MAX_DLEN; i++) random_canal.data[i] = (uint8_t)rand();
	random_canal.timestamp = 0;
	return random_canal;
}

char* trimspaces(char *str)
{
	int count = 0;
	for (int i = 0; str[i]; i++)
		if (str[i] != ' ')
			str[count++] = str[i];
	str[count] = '\0';
	return str;
}

structCanalMsg custom_canal_frame(uint32_t msgid, uint32_t msgflags, char* payload) {
	structCanalMsg custom_canal;
	custom_canal.id = msgid;
	custom_canal.flags = msgflags |swap_endianness(1,1);
	if (msgflags && CANAL_FLAG_XTD) custom_canal.id = msgid & 0x1FFFFFFF;
	else custom_canal.id = msgid & 0x000007FF;
	payload = trimspaces(payload);
	int num_nibbles = strlen(payload);
	if ((num_nibbles % 2) != 0) {
		num_nibbles += 1;
	}
	custom_canal.sizeData = num_nibbles / 2;
	uint64_t hexpayload = (uint64_t)swap_endianness(strtoull(payload, NULL, 16),0) << 32 | swap_endianness(strtoull(payload, NULL, 16) >> 32,0);
	unsigned char *p = (unsigned char*)&hexpayload;
	for (uint8_t i = 0; i < custom_canal.sizeData; i++) {
		custom_canal.data[i] = p[8-custom_canal.sizeData+i];
	}
	for (uint8_t i = custom_canal.sizeData; i < CAN_MAX_DLEN; i++) {
		custom_canal.data[i] = 0;
	}
	custom_canal.timestamp = 0;
	return custom_canal;
}

int encapsulate_socketcan_pcap(PCANALMSG canal_frame, pcappkt_can *pcap_frame)
{
	pcap_pkthdr pcap_packet_header = init_pcap_pkt_header(canal_frame, pcap_frame);
	sll_header sll_header;
	if (canal_frame->flags >> 30) 
		sll_header = init_sll_header(PKTTYPE_CAN_TX);
	else 
		sll_header = init_sll_header(PKTTYPE_CAN_RX);
	
	can_frame socketcan_frame;
	convert_canal_to_socketcan(&socketcan_frame, canal_frame);
	
	pcap_frame->pcap_header = pcap_packet_header;
	pcap_frame->sll_header = sll_header;
	pcap_frame->can_frame = socketcan_frame;

	return 0;
}

struct CanalApi initCanApi(void)
{
	HMODULE usb2canLibrary = LoadLibrary(TEXT("..\\usb2can.dll"));
	struct CanalApi CanalApi;
	CanalApi.CanalOpen = (canOpen)GetProcAddress(usb2canLibrary, "CanalOpen");
	CanalApi.CanalBlockingReceive = (canRcvBlk)GetProcAddress(usb2canLibrary, "CanalBlockingReceive");
	CanalApi.CanalBlockingSend = (canSendBlk)GetProcAddress(usb2canLibrary, "CanalBlockingSend");
	CanalApi.CanalClose = (canClose)GetProcAddress(usb2canLibrary, "CanalClose");
	CanalApi.CanalDataAvailable = (canDataAvailable)GetProcAddress(usb2canLibrary, "CanalDataAvailable");
	CanalApi.CanalGetStatistics = (canGetStatistics)GetProcAddress(usb2canLibrary, "CanalGetStatistics");
	return CanalApi;
}

void set_interface_string(uint32_t current_interface, int baudrate, char * int_string)
{
	char interface_string[15];
	char baudrate_string[6];
	_ltoa_s(current_interface, interface_string, sizeof(interface_string), 16);
	_itoa_s(baudrate, baudrate_string, sizeof(baudrate_string), 10);
	strcat_s(interface_string, sizeof(interface_string), ";");
	strcat_s(interface_string, sizeof(interface_string), baudrate_string);
	strcpy_s(int_string, sizeof(interface_string), interface_string);
}

void init_global_header(pcap_file_header * global_hdr)
{
	global_hdr->magic = 0xA1B2C3D4;
	global_hdr->version_major = 2;
	global_hdr->version_minor = 4;
	global_hdr->thiszone = 0; /* gmt to local correction */
	global_hdr->sigfigs = 0;    /* accuracy of timestamps */
	global_hdr->snaplen = 40;    /* max length saved portion of each pkt */
	global_hdr->linktype = LINKTYPE_LINUX_SLL;
}

void sendCanalStatistics(structCanalStatistics* can_stats, HANDLE writepipe)
{
	char buffer[512];
	int j;
	j =  sprintf_s(buffer, 512,         "Received frames:    %d\n", can_stats->cntReceiveFrames);
	j += sprintf_s(buffer + j, 512 - j, "Transmitted frames: %d\n", can_stats->cntTransmitFrames);
	j += sprintf_s(buffer + j, 512 - j, "Bytes received:     %d\n", can_stats->cntReceiveData);
	j += sprintf_s(buffer + j, 512 - j, "Bytes sent:         %d\n", can_stats->cntTransmitData);
	j += sprintf_s(buffer + j, 512 - j, "Overruns:           %d\n", can_stats->cntOverruns);
	j += sprintf_s(buffer + j, 512 - j, "Bus warnings:       %d\n", can_stats->cntBusWarnings);
	j += sprintf_s(buffer + j, 512 - j, "Bus off:            %d\n", can_stats->cntBusOff);
	HANDLE write_pipe = connect_to_pipe(ctrlout);
	ctrlPkt control_packet;
	control_packet.sync = 'T';
	control_packet.ctrlNum = '5';
	//TODO
}

int capture_can_pcap(uint32_t current_interface, uint32_t flags, int baudrate)
{
	uint32_t ret;
	canalMsg canal_message;
	PCANALMSG pcanal_message = &canal_message;
	HMODULE usb2canLibrary = LoadLibrary(TEXT("..\\usb2can.dll"));
	char interface_string[15];
	set_interface_string(current_interface, baudrate, interface_string);
	struct CanalApi CanalApi = initCanApi();
	canal_handle = CanalApi.CanalOpen(interface_string, flags);
	struct pcap_file_header global_header;
	init_global_header(&global_header);
	pcappkt_can pcap_pkt;
	
	DWORD numWritten;
	HANDLE hPipe = connect_to_pipe(fifo);
	int error;
	error = WriteFile(hPipe, &global_header, sizeof(pcap_file_header), &numWritten, NULL);

	if (!threads_started) {
		if (ctrlin != NULL) {
			_beginthread(&CtrlReadThread, NULL, ctrlin);
			threads_started = 1;
		}
	}
	HANDLE signal_pipe = open_signal_pipe(); // ???????????? WTF ??

	//printf("canhandle %X", canal_handle);
	if (error == 0) printf("could not write header to pipe");
	while (onCapture) {

		if (CanalApi.CanalDataAvailable(canal_handle)) {
			ret = CanalApi.CanalBlockingReceive(canal_handle, pcanal_message, 500);
			encapsulate_socketcan_pcap(pcanal_message, &pcap_pkt);
			error = WriteFile(hPipe, &pcap_pkt, sizeof(pcappkt_can), &numWritten, NULL);
		}
		else if (send_can_message) {
			if (msgid && payload) canal_message = custom_canal_frame(msgid, msgflags, payload);
			else canal_message = random_canal_frame();
			ret = CanalApi.CanalBlockingSend(canal_handle, pcanal_message, 500);
			if (!ret) {
				encapsulate_socketcan_pcap(pcanal_message, &pcap_pkt);
				error = WriteFile(hPipe, &pcap_pkt, sizeof(pcappkt_can), &numWritten, NULL);
			}
			send_can_message = send_can_message - 1;
		}
	}
	return ret;
}