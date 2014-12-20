/*
 * clScanner.h
 *
 *  Created on: Dec 17, 2014
 *      Author: jbw
 */

#ifndef CLSCANNER_H_
#define CLSCANNER_H_

#include "clModbusTCP.h"
#include <thread>
#include <iostream>

class clScanner
{
public:
    clScanner(
        int,                // uid
        const char *,       // ip addr
        int,                // read mem addr location
        int,                // read mem size location
        int,                // write mem addr location
        int);               // write mem size location
    virtual ~clScanner();

    void Execute();

private:

    clModbusTCP  * mp_modbus;
    std::thread scannerthread;

    int m_readmemaddr;
    int m_readmemsize;
    int m_writememaddr;
    int m_writememsize;
};

#endif /* CLSCANNER_H_ */
