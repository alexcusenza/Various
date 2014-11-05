/*
 * clModbusTCP.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#include "clModbusTCP.h"
#include <iostream>

/*
 * ==============================================
 * Constructor / Destructor
 *
 * ==============================================
 */

clModbusTCP::clModbusTCP(
    int uid,
    const char * ipPLC):
    m_uid(uid),
    m_ip(ipPLC)
{
    mp_socket = new clSocket(m_ip);

}

clModbusTCP::~clModbusTCP()
{
    // TODO Auto-generated destructor stub
}


/*
 * ==============================================
 * Modbus Function 02h
 * READ DISCRETE INPUTS
 *
 * ==============================================
 */

int clModbusTCP::read_02h(int addr, int numbits, unsigned char *dest)
{
    const int func = MODBUS_FC_READ_DISCRETE_INPUTS;

    // Check max of data num_
    if (numbits > MODBUS_MAX_READ_BITS)
    {
        printf("ERROR Too many discrete inputs requested: %d\n",
                numbits);
        return -1;
    }

    // Build Message
    req_length = _buildmessage(
            func,
            6,                  // Length: 6 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits)
            addr,
            numbits,
            req_msg);

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        int bytecount;
        bytecount = _checkresponse(
                func,
                rsp_msg);

        if (bytecount < 0)
            return -1;

        for (int i = 0; i < bytecount; i++)
        {
            dest[i] = rsp_msg[8+i];
        }
    }

    return ret;
}

/*
 * ==============================================
 * Modbus Function 03h
 * READ HOLDING REGISTERS
 *
 * ==============================================
 */

int clModbusTCP::read_03h(int addr, int numwords, unsigned char *dest)
{
    const int func = MODBUS_FC_READ_HOLDING_REGISTERS;

    // Check max of data num_
    if (numwords > MODBUS_MAX_READ_REGISTERS)
    {
        printf("ERROR Too many registers requested: %d\n",
                numwords);
        return -1;
    }

    // Build Message
    req_length = _buildmessage(
            func,
            6,                  // 6 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits)
            addr,
            numwords,
            req_msg);

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        int bytecount;
        bytecount = _checkresponse(
                func,
                rsp_msg);

        if (bytecount < 0)
            return -1;

        for (int i = 0; i < bytecount; i++)
        {
            dest[i] = (rsp_msg[8+i*2] << 8) | rsp_msg[9+i*2];
        }
    }
    return ret;
}


/*
 * ==============================================
 * Modbus Function 0fh
 * WRITE MULTIPLE COILS
 *
 * ==============================================
 */

int clModbusTCP::write_0fh(int addr, int numbits, unsigned char *src)
{
    const int func = MODBUS_FC_WRITE_MULTIPLE_COILS;
    int numbytes = (numbits / 8) + ((numbits % 8) ? 1 : 0);
    int length = 7 + numbytes;

    // Check max of data num_
    if (numbits > MODBUS_MAX_WRITE_BITS)
    {
        printf("ERROR: Too many bits: %d\n",
                numbits);
        return -1;
    }

    // Build Message
    req_length = _buildmessage(
            func,
            length,             // 7 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits) + 1(bytes)
            addr,
            numbits,
            req_msg);
    req_msg[req_length++] = numbytes;


    for (int i = 0; i < numbytes; i++)
    {
        req_msg[req_length+i] = src[i];
    }

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        int bytecount;
        bytecount = _checkresponse(
                func,
                rsp_msg);

    }
    return ret;
}

/*
 * ==============================================
 * Modbus Function 10h
 * WRITE MULTIPLE REGISTERS
 *
 * ==============================================
 */

int clModbusTCP::write_10h(int addr, int numwords, unsigned char *src)
{
    const int func = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
    int numbytes = numwords * 2;
    int length = 7 + numbytes;

    // Check max of data num_
    if (numwords > MODBUS_MAX_WRITE_REGISTERS)
    {
        printf("ERROR: Too many registers: %d\n",
                numwords);
        return -1;
    }

    // Build Message
    req_length = _buildmessage(
            func,
            7,                  // 7 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits) + 1(bytes)
            addr,
            numwords,
            req_msg);
    req_msg[req_length++] = numbytes;

    for (int i = 0; i < numbytes; i++)
    {
        req_msg[req_length++] = src[i] >> 8;
        req_msg[req_length++] = src[i] & 0x00FF;
    }

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        int bytecount;
        bytecount = _checkresponse(
                func,
                rsp_msg);

    }

    return ret;
}

/*
 * ==============================================
 * _buildmessage
 *
 * ==============================================
 */

int clModbusTCP::_buildmessage(
    int func,
    int len,
    int addr,
    int count,
    unsigned char *buf)
{
    // Transaction ID
    buf[0] = 0;
    buf[1] = 0;

    // Protocol
    buf[2] = 0;
    buf[3] = 0;

    // Length
    buf[4] = len >> 8;
    buf[5] = len & 0x0ff;

    // Unit
    buf[6] = m_uid;

    // Function
    buf[7] = func;

    // Address
    buf[8] = addr >> 8;
    buf[9] = addr & 0x00ff;

    // Number of bytes
    buf[10] = count >> 8;
    buf[11] = count & 0x0ff;

    return MODBUS_TCP_PRESET_REQ_LENGTH;

}

/*
 * ==============================================
 * _checkresponse
 *
 * ==============================================
 */

int clModbusTCP::_checkresponse(
    const int func,
    unsigned char * rsp)
{
    int uid = rsp[7];
    int function = rsp[8];
    int bytecount = rsp[9];

    if (uid != m_uid ||
            function != func)
    {
        printf("Modbus Error: UID and/or Function Not Matching");

        return -1;
    }

    return bytecount;

}
