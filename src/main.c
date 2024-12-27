//
// Created by Gediminas Simanskis on 25/12/2024.
//

/* gopt-usage.c   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

/* <http:///www.purposeful.co.uk/software/gopt> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/gopt.h"
#include "../include/main.h"

int main (int argc, char **argv)
{
    struct option options[64];
    int32_t interface = -1;
    struct _interface_parameters interface_parameters[8]={};

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

    options[EXTCAP_DLTS].long_name  = "extcap-dlsts";
    options[EXTCAP_DLTS].short_name = '9';
    options[EXTCAP_DLTS].flags      = GOPT_ARGUMENT_FORBIDDEN;

    options[PARAMETER_BITRATE].long_name  = "bitrate";
    options[PARAMETER_BITRATE].short_name = 'A';
    options[PARAMETER_BITRATE].flags      = GOPT_ARGUMENT_REQUIRED;

    options[PARAMETER_SILENT].long_name  = "silent";
    options[PARAMETER_SILENT].short_name = 'B';
    options[PARAMETER_SILENT].flags      = GOPT_ARGUMENT_REQUIRED;

    options[PARAMETER_LOOPBACK].long_name  = "loopback";
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
    gopt_errors (argv[0], options);

    // extcap-interfaces
    if (options[EXTCAP_INTERFACES].count)
    {
        printf("extcap {version=1.0}{help=https://www.rusoku.org}{display=RUSOKU CAN USB adapter extcap interface}\n");
        printf("interface {value=0}{display=Toucan CAN adapter interface - (VIRTUAL DEMO RANDOM PACKETS)}\n");
        exit (EXIT_SUCCESS);
    }

    // extcap-interface
    if (options[EXTCAP_INTERFACE].count)
    {
        interface = (int32_t)strtoll(options[1].argument, NULL, 16);
    }

    //extcap-version
    if (options[EXTCAP_VERSION].count)
    {
        printf("extcap {version=0.1.0}{help=file:///Applications/Wireshark.app/Contents/Resources/share/wireshark/rusoku-wireshark-plugin.html}\n");
        exit (EXIT_SUCCESS);
    }

    // extcap-config
    if (options[EXTCAP_CONFIG].count)
    {
        if ( interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }
		printf("arg {number=0}{call=--parameter_bitrate}{display=Bitrate, kbps}{tooltip=Capture bitrate}{type=integer}{range=10,1000}{required=true}{default=125}\n");
		printf("arg {number=1}{call=--parameter_silent}{display=Silent}{tooltip=enable silent mode}{type=boolflag}\n");
		printf("arg {number=2}{call=--parameter_loopback}{display=Loopback}{tooltip=enable loopback mode}{type=boolflag}\n");
        exit (EXIT_SUCCESS);
    }

    // extcap-dlts
    if (options[EXTCAP_DLTS].count)
    {
        if ( interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }

        if (interface == 0) {
            printf("dlt {number=147}{name=USER0}{display=Demo Implementation for Extcap}");
        }
        else {
            printf("dlt {number=148}{name=USER1}{display=Demo Implementation for Extcap}");
        }
        exit (EXIT_SUCCESS);
    }

    //extcap-bitrate
    if (options[PARAMETER_BITRATE].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }
        interface_parameters[interface].bitrate = (int32_t)strtoll(options[10].argument, NULL, 16);
        exit (EXIT_SUCCESS);
    }

    //extcap-silent
    if (options[PARAMETER_SILENT].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }
        interface_parameters[interface].silent = (int32_t)strtoll(options[11].argument, NULL, 16);
        exit (EXIT_SUCCESS);
    }

    //extcap-loopback
    if (options[PARAMETER_LOOPBACK].count)
    {
        if (interface < 0 || interface > 7) {
            exit (EXIT_FAILURE);
        }
        interface_parameters[interface].loopback = (int32_t)strtoll(options[12].argument, NULL, 16);
        exit (EXIT_SUCCESS);
    }

    return 0;
}
