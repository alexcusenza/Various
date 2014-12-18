/*
 * clModbusTCP.cvgfdx   pp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#include "clModbusTCP.h"
#include <iostream>

/* ==============================================
 * Constructor / Destructor
 *
 * ==============================================
 */

clModbusTCP::clModbusTCP(
    int uid,
    const char * ipAddr):
    m_uid(uid)
{
    mp_socket = new clSocket(ipAddr);
    req_msg = new unsigned char[128];
    rsp_msg = new unsigned char[128];
}

clModbusTCP::~clModbusTCP()
{
    delete req_msg;
    delete rsp_msg;
}

/* ==============================================
 * Modbus Function 02h
 * READ DISCRETE INPUTS
 *
 * ==============================================
 */

int clModbusTCP::read_02h(
    int addr,
    int numbits,
    unsigned char *dest)
{
    int retval;
    int bytecount;
    const int func = MODBUS_FC_READ_DISCRETE_INPUTS;
    int length = MODBUS_TCP_HEADER_READ_LENGTH;
    int numbytes = (numbits / 8) + ((numbits % 8) ? 1 : 0);

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

    rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH + numbytes;

    // Send Request Message
    retval = mp_socket->SendMessage(req_msg, req_length);
    if (retval < 0)
        return -1;

    // Receive Response Message
    retval = mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    if (retval < 0)
        return -1;

    // Check Response
    retval = _checkresponse(func);
    if (retval < 0)
        return -1;

    bytecount = retval;

    // Response Message
    for (int i = 0; i < bytecount; i++)
    {
        dest[i] = rsp_msg[9+i];
    }

    return 1;
}

/* ==============================================
 * Modbus Function 03h
 * READ HOLDING REGISTERS
 *
 * ==============================================
 */

int clModbusTCP::read_03h(
    int addr,
    int numwords,
    unsigned char *dest)
{
    int retval;
    int bytecount;
    const int func = MODBUS_FC_READ_HOLDING_REGISTERS;
    int length = MODBUS_TCP_HEADER_READ_LENGTH;
    int numbytes = numwords * 2;

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

    rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH + numbytes;

    // Send Request Message
    retval = mp_socket->SendMessage(req_msg, req_length);
    if (retval < 0)
        return -1;

    // Receive Response Message
    retval = mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    if (retval < 0)
        return -1;

    // Check Response
    retval = _checkresponse(func);
    if (retval < 0)
        return -1;

    bytecount = retval;

    // Response Message
    for (int i = 0; i < bytecount; i++)
    {
        dest[i] = (rsp_msg[9+i*2] << 8) | rsp_msg[10+i*2];
    }

    return 1;
}


/* ==============================================
 * Modbus Function 0fh
 * WRITE MULTIPLE COILS
 *
 * ==============================================
 */

int clModbusTCP::write_0fh(
    int addr,
    int numbits,
    unsigned char *src)
{
    int retval;
    const int func = MODBUS_FC_WRITE_MULTIPLE_COILS;
    int numbytes = (numbits / 8) + ((numbits % 8) ? 1 : 0);
    int length = MODBUS_TCP_HEADER_WRITE_LENGTH + numbytes;

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

    // Add data to message
    for (int i = 0; i < numbytes; i++)
    {
        req_msg[req_length++] = src[i];
    }

    rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH;

    // Send Request Message
    retval = mp_socket->SendMessage(req_msg, req_length);
    if (retval < 0)
        return -1;

    // Receive Response Message
    retval = mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    if (retval < 0)
        return -1;

    // Check Response
    retval = _checkresponse(func);
    if (retval < 0)
        return -1;

    return 1;
}

/* ==============================================
 * Modbus Function 10h
 * WRITE MULTIPLE REGISTERS
 *
 * ==============================================
 */

int clModbusTCP::write_10h(
    int addr,
    int numwords,
    unsigned char *src)
{
    int retval;
    const int func = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
    int numbytes = numwords * 2;
    int length = MODBUS_TCP_HEADER_WRITE_LENGTH + numbytes;

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

    rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH;

    // Send Request Message
    retval = mp_socket->SendMessage(req_msg, req_length);
    if (retval < 0)
        return -1;

    // Receive Response Message
    retval = mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    if (retval < 0)
        return -1;

    // Check Response
    retval = _checkresponse(func);
    if (retval < 0)
        return -1;

    return 1;
}

/* ==============================================
 * _buildmessage
 * PRIVATE
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
    req_msg[5] = len & 0x00ff;

    // Unit
    req_msg[6] = (unsigned char) m_uid;

    // Function
    req_msg[7] = (unsigned char) func;

    // Address
    req_msg[8] = addr >> 8;
    req_msg[9] = addr & 0x00ff;

    // Number of bytes
    req_msg[10] = count >> 8;
    req_msg[11] = count & 0x00ff;

    return MODBUS_TCP_PRESET_REQ_LENGTH;
}

/* ==============================================
 * _checkresponse
 * PRIVATE
 * ==============================================
 */

int clModbusTCP::_checkresponse(
    const int func)
{
    int uid = rsp_msg[6];
    int function = rsp_msg[7];
    int bytecount = rsp_msg[8];

    if (uid != m_uid)
    {
        printf("Modbus Error: UID Error \n");
        return -1;
    }
    else if (function == func + 0x0080)
    {
        printf("Modbus Error: Function %x, error %d \n", function, rsp_msg[8]);
        return -1;
    }

    return bytecount;
}
