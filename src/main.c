//
// Created by Gediminas Simanskis on 25/12/2024.
//

/* gopt-usage.c   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

/* <http:///www.purposeful.co.uk/software/gopt> */

//https://ask.wireshark.org/question/29329/extcap-how-to-use-messages-control-protocol/
//https://github.com/openthread/pyspinel/blob/main/extcap_ot.py

#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include <unistd.h>
#include "../include/gopt.h"
#include "../include/main.h"

#include <_time.h>
#include <string.h>

#define _DEBUG_LOCAL

int main (int argc, char **argv)
{
    struct option options[17];
    int32_t interface = -1;
    struct _interface_parameters interface_parameters[8] = {};

    //struct _pcap_file_header    pcap_file_hdr = {};
    //struct _pcap_packet_header  pcap_packet_hdr = {};
    //struct _sll_packet_header  sll_packet_hdr = {};
    //printf("PCAP file header=%ld\n", sizeof(pcap_file_hdr));
    //printf("PCAP packet header=%ld\n", sizeof(pcap_packet_hdr));
    //printf("SLL packet header=%ld\n", sizeof(sll_packet_hdr));

    options[EXTCAP_INTERFACES].long_name  = "extcap-interfaces";
    options[EXTCAP_INTERFACES].short_name = '0';
    options[EXTCAP_INTERFACES].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[EXTCAP_INTERFACE].long_name  = "extcap-interface";
    options[EXTCAP_INTERFACE].short_name = '1';
    options[EXTCAP_INTERFACE].flags      = GOPT_ARGUMENT_REQUIRED;

    options[EXTCAP_VERSION].long_name  = "extcap-version";
    options[EXTCAP_VERSION].short_name = '2';
    options[EXTCAP_VERSION].flags      = GOPT_ARGUMENT_OPTIONAL;

    options[EXTCAP_CONFIG].long_name  = "extcap-config";
    options[EXTCAP_CONFIG].short_name = '3';
    options[EXTCAP_CONFIG].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[EXTCAP_CAPTURE_FILTER].long_name  = "extcap-capture-filter";
    options[EXTCAP_CAPTURE_FILTER].short_name = '4';
    options[EXTCAP_CAPTURE_FILTER].flags      = GOPT_ARGUMENT_OPTIONAL;

    options[FIFO].long_name  = "fifo";
    options[FIFO].short_name = '5';
    options[FIFO].flags      = GOPT_ARGUMENT_REQUIRED;

    options[EXTCAP_CONTROL_IN].long_name  = "extcap-control-in";
    options[EXTCAP_CONTROL_IN].short_name = '6';
    options[EXTCAP_CONTROL_IN].flags      = GOPT_ARGUMENT_REQUIRED;

    options[EXTCAP_CONTROL_OUT].long_name  = "extcap-control-out";
    options[EXTCAP_CONTROL_OUT].short_name = '7';
    options[EXTCAP_CONTROL_OUT].flags      = GOPT_ARGUMENT_REQUIRED;

    options[EXTCAP_RELOAD_OPTION].long_name  = "extcap-reload-option";
    options[EXTCAP_RELOAD_OPTION].short_name = '8';
    options[EXTCAP_RELOAD_OPTION].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[EXTCAP_DLTS].long_name  = "extcap-dlts";
    options[EXTCAP_DLTS].short_name = '9';
    options[EXTCAP_DLTS].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[PARAMETER_BITRATE].long_name  = "parameter_bitrate";
    options[PARAMETER_BITRATE].short_name = 'A';
    options[PARAMETER_BITRATE].flags      = GOPT_ARGUMENT_REQUIRED;

    options[PARAMETER_SILENT].long_name  = "parameter_silent";
    options[PARAMETER_SILENT].short_name = 'B';
    options[PARAMETER_SILENT].flags      = GOPT_ARGUMENT_REQUIRED;

    options[PARAMETER_LOOPBACK].long_name  = "parameter_loopback";
    options[PARAMETER_LOOPBACK].short_name = 'C';
    options[PARAMETER_LOOPBACK].flags      = GOPT_ARGUMENT_REQUIRED;

    options[LOG_LEVEL].long_name  = "log-level";
    options[LOG_LEVEL].short_name = 'D';
    options[LOG_LEVEL].flags      = GOPT_ARGUMENT_REQUIRED;

    options[LOG_FILE].long_name  = "log-file";
    options[LOG_FILE].short_name = 'E';
    options[LOG_FILE].flags      = GOPT_ARGUMENT_REQUIRED;

    options[CAPTURE].long_name  = "capture";
    options[CAPTURE].short_name = 'F';
    options[CAPTURE].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[GOPT_LAST_OPT].flags = GOPT_LAST;

#ifdef _DEBUG_LOCAL
    //------------- DEBUG --------------

    //FILE *fout;
    //fout = fopen ("a.log", "a");
    //for (int i = 1; i < argc; i++)
    //{
   //     fputs (argv[i], fout);
   //     fputs (" ",    fout);
   //}
   // fputs ("\n",fout);
    //fflush (fout);
    //fclose (fout);

    //--------------------------------------
#endif

    argc = gopt (argv, options);
    //gopt_errors (argv[0], options);

    // extcap-interfaces
    if (options[EXTCAP_INTERFACES].count)
    {
        //fprintf(fout," EXTCAP_INTERFACES ");

        //printf("extcap {version=1.0}{help=https://www.rusoku.org}{display=RUSOKU CAN USB adapter extcap interface}\n");
        printf("extcap {version=0.1}\n");
        printf("interface {value=0}{display=Toucan CAN adapter interface 0 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=1}{display=Toucan CAN adapter interface 1 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=2}{display=Toucan CAN adapter interface 2 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=3}{display=Toucan CAN adapter interface 3 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=4}{display=Toucan CAN adapter interface 4 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=5}{display=Toucan CAN adapter interface 5 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //printf("interface {value=6}{display=Toucan CAN adapter interface 6 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        //rintf("interface {value=7}{display=Toucan CAN adapter interface 7 - (VIRTUAL DEMO RANDOM PACKETS)}\n");

        //printf("control {number=0}{type=string}{display=Msg ID}{tooltip=Custom frame message ID (0xNNNNNNNN)}{validation=^(([01][a-fA-F0-9]\\{0,7\\})|([a-fA-F0-9]\\{0,7\\}))$}\n");
        //printf("control {number=1}{type=boolean}{display=Extended}{tooltip=Extended CAN frame}{default=true}\n");
        //printf("control {number=2}{type=boolean}{display=RTR}{tooltip=Request to Re-transmit}{default=false}\n");
        //printf("control {number=3}{type=string}{display=Message}{tooltip=Custom message (0-8 bytes, space separated)}{validation=^([a-fA-F0-9]\\{0,2\\}(\\s)*)\\{0,8\\}$}\n");
        //printf("control {number=4}{type=button}{display=Send}{tooltip=Send custom frame if values provided. If empty, sends a random frame}\n");

        //printf("value {control=1}{value=1}{display=1 sec}");
        //printf("value {control=1}{value=2}{display=2 sec}{default=true}");

        //exit (EXIT_SUCCESS);
    }

    // extcap-interface
    if (options[EXTCAP_INTERFACE].count)
    {
        //fprintf(fout," EXTCAP_INTERFACE = %s ",options[EXTCAP_INTERFACE].argument);
        interface = (int32_t)strtoll(options[EXTCAP_INTERFACE].argument, NULL, 16);
    }

    //extcap-version
    if (options[EXTCAP_VERSION].count)
    {
        //fprintf(fout," EXTCAP_VERSION ");
        printf("extcap {version=1.0}{help=file:///Applications/Wireshark.app/Contents/Resources/share/wireshark/rusoku-wireshark-plugin.html}\n");
        //printf("extcap {version=1.0}{help=https://www.rusoku.org}{display=RUSOKU CAN USB adapter extcap interface}\n");
    }

    // extcap-config
    if (options[EXTCAP_CONFIG].count)
    {
        //fprintf(fout," EXTCAP_CONFIG ");
        //fprintf(fout," IFACE = %s ",options[EXTCAP_INTERFACE].argument);

     //   if ( interface < 0 || interface > 7) {
     //       exit (EXIT_FAILURE);
     //   }
		printf("arg {number=0}{call=--parameter_bitrate}{display=Bitrate, kbps}{tooltip=Capture bitrate}{type=integer}{range=10,1000}{required=true}{default=125}\n");
		printf("arg {number=1}{call=--parameter_silent}{display=Silent}{tooltip=enable silent mode}{type=boolflag}\n");
		printf("arg {number=2}{call=--parameter_loopback}{display=Loopback}{tooltip=enable loopback mode}{type=boolflag}\n");
    }

    // extcap-dlts
    if (options[EXTCAP_DLTS].count)
    {
        //fprintf(fout," EXTCAP_DLTS ");

        //printf("dlt {number=113}{name=DLT_LINUX_SLL}{display=TouCAN}\n");
        printf("dlt {number=147}{name=USER0}{display=Demo Implementation for Extcap}\n");
    }

    //extcap-bitrate
    if (options[PARAMETER_BITRATE].count)
    {
        //fprintf(fout," PARAMETER_BITRATE ");

        //if (interface < 0 || interface > 7) {
        //    exit (EXIT_FAILURE);
        //}
        interface_parameters[interface].bitrate = (int32_t)strtoll(options[PARAMETER_BITRATE].argument, NULL, 16);
    }

    //extcap-silent
    if (options[PARAMETER_SILENT].count)
    {
        //fprintf(fout," PARAMETER_SILENT ");

        //if (interface < 0 || interface > 7) {
        //    exit (EXIT_FAILURE);
        //}
        interface_parameters[interface].silent = (int32_t)strtoll(options[PARAMETER_SILENT].argument, NULL, 16);
    }

    //extcap-loopback
    if (options[PARAMETER_LOOPBACK].count)
    {
        //fprintf(fout," PARAMETER_LOOPBACK ");

       // if (interface < 0 || interface > 7) {
       //     exit (EXIT_FAILURE);
       // }
        interface_parameters[interface].loopback = (int32_t)strtoll(options[PARAMETER_LOOPBACK].argument, NULL, 16);
    }

    //fifo
    if (options[FIFO].count)
    {
        //fprintf(fout, "FIFO");

        //if (interface < 0 || interface > 7) {
        //    exit (EXIT_FAILURE);
       //}
    }

    //capture
    if (options[CAPTURE].count)
    {
        //fprintf(fout," CAPTURE ");
    }

    //fprintf(fout,"\n");
    //fflush(fout);
    //fclose(fout);

    return (EXIT_SUCCESS);
}
