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

const char * IP_BECKHOFF2 = "172.16.17.76";
const int UID_BECKHOFF2 = 0;

const char * IP_SICK0 = "172.16.17.50";
const int UID_SICK0 = 1;


int main (void)
{
    clScanner cBeckhoff(UID_BECKHOFF, IP_BECKHOFF, 0, 4, 0, 4);
    clScanner cBeckhoff2(UID_BECKHOFF2, IP_BECKHOFF2, 0, 4, 0, 4);

    while(1)
    {

    }


    return 0;
}
