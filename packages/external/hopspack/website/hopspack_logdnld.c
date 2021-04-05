/*******************************************************/
/* Copyright (c) 2009-2010 Sandia Corporation          */
/* All Rights Reserved                                 */
/*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>


static const char  LOGFILE[] = "hopspack_files/hopspack_dnld_log.txt";


/** Return the decimal value corresponding to a hex digit.
 */
static int  toHexValue (const char  c)
{
    if ((c >= '0') && (c <= '9'))
        return( ((int) c) - ((int) '0') );
    if ((c >= 'A') && (c <= 'F'))
        return( 10 + ((int) c) - ((int) 'A') );
    if ((c >= 'a') && (c <= 'f'))
        return( 10 + ((int) c) - ((int) 'f') );

    return( -1 );
}


/** Unencode from URL encoding (very simple implementation).
 */
static void  urlUnencode (const char * const  szEncoded,
                                char * const  szUnencoded,
                          const int           nMaxChars)
{
    int  k;

    if (szEncoded == NULL)
    {
        sprintf (szUnencoded, "(null)");
        return;
    }

    int  i = 0;
    int  j = 0;
    while (szEncoded[i] != 0)
    {
        if (szEncoded[i] == '%')
        {
            if ((szEncoded[i+1] != 0) && (szEncoded[i+2] != 0))
            {
                k = 16 * toHexValue (szEncoded[i+1])
                       + toHexValue (szEncoded[i+2]);
                szUnencoded[j] = (char) k;
                i += 3;
                j++;
            }
            else    /*-- ERRONEOUS ENCODING, BUT KEEP GOING */
            {
                szUnencoded[j] = szEncoded[i];
                i++;
                j++;
            }
        }
        else if (szEncoded[i] == '+')
        {
            szUnencoded[j] = ' ';
            i++;
            j++;
        }
        else
        {
            szUnencoded[j] = szEncoded[i];
            i++;
            j++;
        }

        if (j >= nMaxChars)
            break;
    }

    szUnencoded[j] = 0;
    return;
}


/** This program is invoked as a CGI module from the download page that
 *  is generated by hopspack_gendownload.c.
 */
int  main (int argc, char *argv[])
{
    char    szUnencQuery[1000 + 1];
    int     i;
    FILE *  fp;
    time_t  rawTime;
    char    szTime[30];


    /*---- READ THE QUERY STRING. */
    urlUnencode (getenv ("QUERY_STRING"), szUnencQuery, 1000);
    szUnencQuery[1000] = 0;

    /*---- TIME STAMP HAS FORMAT ddd mmm DD HH:MM:SS YYYY. */
    time (&rawTime);
    asctime_r (localtime (&rawTime), szTime);
    szTime[strlen (szTime) - 1] = 0;

    fp = fopen (LOGFILE, "a");
    fprintf (fp, "%s QUERY:%s HTTP_USER_AGENT=%s REMOTE_ADDR=%s\n",
             szTime,
             szUnencQuery,
             getenv ("HTTP_USER_AGENT"),
             getenv ("REMOTE_ADDR"));
    fclose (fp);

    /*---- SEND A BLANK PAGE TO SATISFY APACHE. */
    printf ("Content-type: text/html; charset=utf-8%c%c", 10, 10);
    printf ("<html><head></head><body></body></html>%c%c", 10, 10);
    fflush (stdout);

    return( 0 );
}
