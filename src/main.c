//
// Created by Gediminas Simanskis on 25/12/2024.
//

/* gopt-usage.c   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

/* <http:///www.purposeful.co.uk/software/gopt> */

/*
Links[...]
https://ask.wireshark.org/question/29329/extcap-how-to-use-messages-control-protocol/
https://github.com/openthread/pyspinel/blob/main/extcap_ot.py
https://discussions.apple.com/thread/255759797?sortBy=rank
https://github.com/secdev
https://www.ietf.org/archive/id/draft-gharris-opsawg-pcap-00.html#name-packet-record
*/

#include "../include/gopt.h"
#include "../include/main.h"
#include "../include/capture.h"

/*
    ~/Library/Logs for user-oriented info from non-system software
    /Library/Logs for Mac-ish system-wide event logging
    /var/log for Unix-ish system-wide event logging
*/

int main (int argc, char **argv)
{
    struct option options[17];
    int interface = -1;
    struct INTERFACE_PARAMETERS interface_parameters[8] = {};

    char *fifo_name;
    FILE *fd_fifo;

    //struct PCAP_FILE_HEADER pcap_file_header = {};

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

    argc = gopt (argv, options);

    // extcap-interfaces
    if (options[EXTCAP_INTERFACES].count)
    {
        printf("extcap {version=1.0}{help=https://www.rusoku.org}{display=RUSOKU CAN USB adapter extcap interface}\n");
        printf("interface {value=0}{display=Toucan CAN adapter interface 0 - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        printf("interface {value=1}{display=Toucan CAN adapter interface 1 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=2}{display=Toucan CAN adapter interface 2 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=3}{display=Toucan CAN adapter interface 3 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=4}{display=Toucan CAN adapter interface 4 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=5}{display=Toucan CAN adapter interface 5 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=6}{display=Toucan CAN adapter interface 6 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=7}{display=Toucan CAN adapter interface 7 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");
        printf("interface {value=8}{display=Toucan CAN adapter interface 8 - (RUSOKU TouCAN sn: xxxxxxxx )}\n");

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
        interface = (int32_t)strtoll(options[EXTCAP_INTERFACE].argument, NULL, 16);
    }

    //extcap-version
    if (options[EXTCAP_VERSION].count)
    {
        //printf("extcap {version=1.0}{help=file:///Applications/Wireshark.app/Contents/Resources/share/wireshark/rusoku-wireshark-plugin.html}\n");
        printf("extcap {version=1.0}{help=https://www.rusoku.org}{display=RUSOKU CAN USB adapter extcap interface}\n");
    }

    // extcap-config
    if (options[EXTCAP_CONFIG].count)
    {
        if ( interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }
		printf("arg {number=0}{call=--parameter_bitrate}{display=Bitrate, kbps}{tooltip=Capture bitrate}{type=integer}{range=10,1000}{required=true}{default=125}\n");
		printf("arg {number=1}{call=--parameter_silent}{display=Silent}{tooltip=enable silent mode}{type=boolflag}{required=true}{default=false}\n");
		printf("arg {number=2}{call=--parameter_loopback}{display=Loopback}{tooltip=enable loopback mode}{type=boolflag}{required=true}{default=false}\n");
    }

    // extcap-dlts
    if (options[EXTCAP_DLTS].count)
    {
        printf("dlt {number=113}{name=DLT_LINUX_SLL}{display=TouCAN}\n");
        //printf("dlt {number=147}{name=USER0}{display=Demo Implementation for Extcap}\n");
    }

    //extcap-bitrate
    if (options[PARAMETER_BITRATE].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }

        interface_parameters[interface].bitrate |= (int32_t)strtoll(options[PARAMETER_BITRATE].argument, NULL, 16);
    }

    //extcap-silent
    if (options[PARAMETER_SILENT].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }

        if ((int32_t)strtoll(options[PARAMETER_SILENT].argument, NULL, 16)) {
            interface_parameters[interface].options |= INTERAFACE_PARAMETER_SILENT;
        }
    }

    //extcap-loopback
    if (options[PARAMETER_LOOPBACK].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }

        if ((int32_t)strtoll(options[PARAMETER_LOOPBACK].argument, NULL, 16)) {
            interface_parameters[interface].options |= INTERAFACE_PARAMETER_LOOPBACK;
        }
    }

    //fifo
    if (options[FIFO].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
       }
        fifo_name = options[FIFO].argument;
    }

    //capture
    if (options[CAPTURE].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }

    capture(fifo_name, interface_parameters);

    }
    return (EXIT_SUCCESS);
}
