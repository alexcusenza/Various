/*
 * test.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

using namespace std;

#include "clModbusTCP.h"

const char * IP_PLC = "172.16.17.75";
const int UID_PLC = 0;

const char * IP_SICK0 = "172.16.17.50";
const int UID_SICK0 = 1;

int main (void)
{
    unsigned char in[32];
    unsigned char out[] = { 1, 0};

    int ret;
    int OUTBITS = 4;
    bool toggle = true;
    int cnt = 0;

    clModbusTCP cBeckhoff(UID_PLC, IP_PLC);
    //clModbusTCP cSickcpu0 = clModbusTCP(UID_SICK0, IP_SICK0);

    while (ret >= 0)
    {
        // TOGGLE
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


        // READ
        ret = cBeckhoff.read_02h(0, 4, in);
        if (ret < 0)
        {
            printf("read error \n");
            return -1;
        }

        // WRITE
        ret = cBeckhoff.write_0fh(0, OUTBITS, out);
        if (ret < 0)
        {
            printf("write error \n");
            return -1;
        }

        // PRINT DISPLAY
        for (int i=0; i<1; i++)
            printf("[%d]:%x ", i, in[i]);
        printf("\n");

        for (int i=0; i<1; i++)
            printf("[%d]:%x ", i, out[i]);
        printf("\n");

        usleep(500*1000);
    }

    return 0;
}
