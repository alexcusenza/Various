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
const int UID_SICK0 = 0;

int main (void)
{
    int ret;
    clModbusTCP cBeckhoff = clModbusTCP(UID_PLC, IP_PLC);
    clModbusTCP cSickcpu0 = clModbusTCP(UID_SICK0, IP_SICK0);

    unsigned char in[128];
    unsigned char out[] = { 1, 0, 1, 0 };


    while (ret >= 0)
    {
        ret = cBeckhoff.read_02h(0, 1, in);
        if (ret < 0)
            printf("read error \n");

        ret = cBeckhoff.write_0fh(0, 4, out);
        if (ret < 0)
            printf("write error \n");

        sleep(1);
    }

    return 0;
}
