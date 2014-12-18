/*
 * test.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

using namespace std;

#include "clScanner.h"

const char * IP_PLC = "172.16.17.75";
const int UID_PLC = 0;

const char * IP_SICK0 = "172.16.17.50";
const int UID_SICK0 = 1;



int main (void)
{
    //clModbusTCP cBeckhoff(UID_PLC, IP_PLC);
    //clModbusTCP cSickcpu0 = clModbusTCP(UID_SICK0, IP_SICK0);
    clScanner(UID_PLC, IP_PLC, 0,4,0,4);

    return 0;
}
