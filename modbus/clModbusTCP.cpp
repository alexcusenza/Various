/*
 * clModbusTCP.cvgfdx   pp
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
    int uid,                // 1
    const char * ipPLC):    // 2
    m_uid(uid),             // 3
    m_ip(ipPLC)             // 4
{
    mp_socket = new clSocket(m_ip);
    req_msg = new unsigned char[256];
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
    int ret;
    int bytecount;
    const int func = MODBUS_FC_READ_DISCRETE_INPUTS;
    int length = 6;             // 6 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits)

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
            length,
            addr,
            numbits);

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        bytecount = _checkresponse(func);

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
    int ret;
    int bytecount;
    const int func = MODBUS_FC_READ_HOLDING_REGISTERS;
    int length = 6;             // 6 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits)

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
            length,
            addr,
            numwords);

    // Send Request Message
    ret = mp_socket->SendMessage(req_msg, req_length);
    if (ret > 0)
    {
        // Receive Response Message
        ret = mp_socket->ReceiveMessage(rsp_msg, rsp_length);

        if (ret == -1)
            return -1;

        bytecount = _checkresponse(func);

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
    int ret;

    const int func = MODBUS_FC_WRITE_MULTIPLE_COILS;
    int numbytes = (numbits / 8) + ((numbits % 8) ? 1 : 0);
    int length = 7 + numbytes;      // 7 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits) + 1(bytes)

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
            length,
            addr,
            numbits);
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

        ret = _checkresponse(func);

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
    int ret;

    const int func = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
    int numbytes = numwords * 2;
    int length = 7 + numbytes;      // 7 bytes = 1(uid) + 1(func) + 2(addr) + 2(bits) + 1(bytes)

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
            length,
            addr,
            numwords);
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

        ret = _checkresponse(func);

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
    int count)
{
    // Transaction ID
    req_msg[0] = 0;
    req_msg[1] = 0;

    // Protocol
    req_msg[2] = 0;
    req_msg[3] = 0;

    // Length
    req_msg[4] = len >> 8;
    req_msg[5] = len & 0x0ff;

    // Unit
    req_msg[6] = (unsigned char) m_uid;

    // Function
    req_msg[7] = (unsigned char) func;

    // Address
    req_msg[8] = addr >> 8;
    req_msg[9] = addr & 0x00ff;

    // Number of bytes
    req_msg[10] = count >> 8;
    req_msg[11] = count & 0x0ff;

    return MODBUS_TCP_PRESET_REQ_LENGTH;

}

/*
 * ==============================================
 * _checkresponse
 *
 * ==============================================
 */

int clModbusTCP::_checkresponse(
    const int func)
{
    int uid = rsp_msg[7];
    int function = rsp_msg[8];
    int bytecount = rsp_msg[9];
    ret_error = 0;

    if (uid != m_uid)
    {
        printf("Modbus Error: UID Error");
        return -1;
    }
    else if (function == func + 0x0080)
    {
        printf("Modbus Error: Function Error");
        ret_error = function;
        return -1;
    }

    return bytecount;

}
