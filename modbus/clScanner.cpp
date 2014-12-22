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
    scannerthread = std::thread(&clScanner::Execute, this);

    in = new unsigned char[8];
    out = new unsigned char[8];
}

clScanner::~clScanner()
{
    scannerthread.join();
    delete mp_modbus;
    delete in;
    delete out;
}

void clScanner::SetWriteData(unsigned char * data)
{
    out =  data;
}

void clScanner::Execute()
{
    int ret=1;

    while(ret>0)
    {
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

    }

}
