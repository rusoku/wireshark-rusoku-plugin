//
// Created by Gediminas Simanskis on 25/12/2024.
//

/* gopt-usage.c   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

/* <http:///www.purposeful.co.uk/software/gopt> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/gopt.h"

/*
#define EXTCAP_PIPE_PREFIX "wireshark_extcap"
#define EXTCAP_CONTROL_IN_PREFIX  "wireshark_control_ext_to_ws"
#define EXTCAP_CONTROL_OUT_PREFIX "wireshark_control_ws_to_ext"

#define EXTCAP_ARGUMENT_CONFIG                  "--extcap-config"
#define EXTCAP_ARGUMENT_RELOAD_OPTION           "--extcap-reload-option"
#define EXTCAP_ARGUMENT_LIST_INTERFACES         "--extcap-interfaces"
#define EXTCAP_ARGUMENT_INTERFACE               "--extcap-interface"
#define EXTCAP_ARGUMENT_LIST_DLTS               "--extcap-dlts"
#define EXTCAP_ARGUMENT_VERSION                 "--extcap-version"

#define EXTCAP_ARGUMENT_RUN_CAPTURE             "--capture"
#define EXTCAP_ARGUMENT_CAPTURE_FILTER          "--extcap-capture-filter"
#define EXTCAP_ARGUMENT_RUN_PIPE                "--fifo"
#define EXTCAP_ARGUMENT_CONTROL_IN              "--extcap-control-in"
#define EXTCAP_ARGUMENT_CONTROL_OUT             "--extcap-control-out"
*/

int main (int argc, char **argv)
{
    struct option options[5];

    options[0].long_name  = "help";
    options[0].short_name = 'h';
    options[0].flags      = GOPT_ARGUMENT_OPTIONAL; //GOPT_ARGUMENT_FORBIDDEN;

    options[1].long_name  = "version";
    options[1].short_name = 'V';
    options[1].flags      = GOPT_ARGUMENT_OPTIONAL; //GOPT_ARGUMENT_FORBIDDEN;

    options[2].long_name  = "verbose";
    options[2].short_name = 'v';
    options[2].flags      = GOPT_ARGUMENT_REQUIRED; //GOPT_ARGUMENT_FORBIDDEN;

    options[3].long_name  = "output";
    options[3].short_name = 'o';
    options[3].flags      = GOPT_ARGUMENT_REQUIRED;

    options[4].flags      = GOPT_LAST;

    argc = gopt (argv, options);
    gopt_errors (argv[0], options);

    FILE *fout;
    int   i;

    if (options[0].count)
    {
        fprintf (stdout, "see the manual\n");
        fprintf (stdout, "option_count=%d\n",options[0].count); //GS
        fprintf (stdout, "option_argument=%s\n",options[0].argument); //GS
        //exit (EXIT_SUCCESS);
    }

    if (options[1].count)
    {
        fprintf (stdout, "version 1.0\n");
        fprintf (stdout, "option_count=%d\n",options[1].count); //GS
        fprintf (stdout, "option_argument=%s\n",options[1].argument); //GS
        //exit (EXIT_SUCCESS);
    }

//***********************************************************************
/*
    struct option
    {
        char          short_name;
        const char   *long_name;
        unsigned int  flags;
        unsigned int  count;
        char         *argument;
    };
*/
    if (options[2].count >= 1)
    {
        fputs ("being verbose\n",stdout /*stderr*/);
        fprintf (stdout, "option_count=%d\n",options[2].count); //GS
        fprintf (stdout, "option_argument=%s\n",options[2].argument); //GS
    }

    if (options[2].count >= 2)
    {
        fputs ("being very verbose\n", stdout /*stderr*/);
        fprintf (stdout, "option_count=%d\n",options[2].count); //GS
    }

//***********************************************************************

    if (options[3].count)
    {
        //fout = fopen (options[3].argument, "w");

        //if (!fout)
        //{
        fprintf (stdout, "option_count=%d\n",options[3].count); //GS
        fprintf (stdout, "option_argument=%s\n",options[3].argument); //GS
            //perror (options[3].argument);
            //exit (EXIT_FAILURE);
        //}
    }
    else
    {
        fout = stdout;
    }

/*
    for (i = 0; i < argc; i++)
    {
        fputs (argv[i], fout);
        fputs ("\n",    fout);
    }
*/

    return 0;
}
