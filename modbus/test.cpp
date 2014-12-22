/*
 * test.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

using namespace std;

#include "clScanner.h"

const char * IP_BECKHOFF = "172.16.17.75";
const int UID_BECKHOFF = 0;
const int READ_ADDR_BECKHOFF = 0;
const int READ_SIZE_BECKHOFF = 4;
const int WRITE_ADDR_BECKOFF = 0;
const int WRITE_SIZE_BECKOFF = 4;

const char * IP_BECKHOFF2 = "172.16.17.76";
const int UID_BECKHOFF2 = 0;

const char * IP_SICK0 = "172.16.17.50";
const int UID_SICK0 = 1;

int main (void)
{
    clScanner cBeckhoff(
        UID_BECKHOFF,
        IP_BECKHOFF,
        READ_ADDR_BECKHOFF,
        READ_SIZE_BECKHOFF,
        WRITE_ADDR_BECKOFF,
        WRITE_SIZE_BECKOFF);

    clScanner cBeckhoff2(
        UID_BECKHOFF2,
        IP_BECKHOFF2,
        0, 4, 0, 4);

    unsigned char out[] = {1,0,0,0};
    int OUTBITS = 4;
    bool toggle = true;
    int cnt = 0;

    unsigned char out2[] = {0,0,0,0};
    int cnt2 = 0;

    while(1)
    {
        // TOGGLE OUTPUT - Beckhoff #1
        if (toggle == true)
            out[0] = out[0] << 1;
        else
            out[0] = out[0] >> 1;

        cnt++;
        if (cnt == OUTBITS-1)
        {
            toggle = !toggle;
            cnt = 0;
        }
        cBeckhoff.SetWriteData(out);


        // TOGGLE OUTPUT - Beckhoff #2
        out2[0] = out2[0] + 1;

        cnt2++;
        if (cnt2 == 16)
        {
            cnt2 = 0;
            out2[0] = 0;
        }
        cBeckhoff2.SetWriteData(out2);

        usleep(250*1000);
    }

    return 0;
}
