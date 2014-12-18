/*
 * clScanner.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: jbw
 */

#include "clScanner.h"


clScanner::clScanner(
    int uid,
    const char * ipaddr,
    int readmemaddr,
    int readmemsize,
    int writememaddr,
    int writememsize):
    m_readmemaddr(readmemaddr),
    m_readmemsize(readmemsize),
    m_writememaddr(writememaddr),
    m_writememsize(writememsize)
{
    mp_modbus = new clModbusTCP(uid, ipaddr);
    std::thread scannerthread(&clScanner::Execute, this);
}

clScanner::~clScanner()
{
    // TODO Auto-generated destructor stub
}

void clScanner::Execute()
{
    int ret;
    int OUTBITS = 4;
    bool toggle = true;
    int cnt = 0;

    unsigned char in[16];
    unsigned char out[] = {1,0,0,0} ;

    while(ret>0)
    {
        // TOGGLE OUTPUT
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
        ret = mp_modbus->read_02h(m_readmemaddr, m_readmemsize, in);
        if (ret < 0)
        {
            printf("read error \n");
            //return -1;
        }

        // WRITE
        ret = mp_modbus->write_0fh(m_writememaddr, m_writememsize, out);
        if (ret < 0)
        {
            printf("write error \n");
            //return -1;
        }

        // PRINT DISPLAY
        for (int i=0; i<1; i++)
            printf("[%d]:%x ", i, in[i]);
        printf("\n");

        for (int i=0; i<1; i++)
            printf("[%d]:%x ", i, out[i]);
        printf("\n");
    }

}
