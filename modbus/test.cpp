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

int main (void)
{
    int ret;
    clModbusTCP cBeckhoff = clModbusTCP(UID_PLC, IP_PLC);

    unsigned char * in;
    unsigned char * out;

    unsigned char arrout[4] = { 1, 0, 1, 0 };
    out = arrout;

    while (ret > 0)
    {
        ret = cBeckhoff.read_02h(0, 1, in);
        if (ret < 0)
            printf("read error");

        ret = cBeckhoff.write_0fh(0, 4, out);
        if (ret < 0)
            printf("write error");

        sleep(1);
    }

    return 0;
}
