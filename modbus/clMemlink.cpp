/*
 * clMemlink.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: jbw
 */

#include "clMemlink.h"

clMemlink::clMemlink(
    int uid,
    const char * ipaddr)
{
    mp_socket = new clSocket(ipaddr);
    req_msg = new unsigned int[128];
    rsp_msg = new unsigned int[128];
}

clMemlink::~clMemlink()
{
    delete req_msg;
    delete rsp_msg;
}


int clMemlink::read(
    int addr,
    int numwords,
    unsigned char * dest)
{
    int retval;
    const int func = 0x52;
    int length = 0;

    // Build Message
    req_length = _buildmessage(
            func,
            length,
            addr,
            numwords);

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


    mp_socket->SendMessage(req_msg, req_length);

    mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    return 1;
}


int clMemlink::write(
    int addr,
    int numwords,
    unsigned char * dest)
{

    mp_socket->SendMessage(req_msg, req_length);

    mp_socket->ReceiveMessage(rsp_msg, rsp_length);
    return 1;
}

int clMemlink::_buildmessage(
    int func,
    int len,
    int addr,
    int count)
{
    // Header
    req_msg[0] = 0x42;
    req_msg[1] = 0;
    req_msg[2] = 0;
    req_msg[3] = 0;

    // Length
    req_msg[4] = 0;
    req_msg[5] = 0;
    req_msg[6] = 0;
    req_msg[7] = len;

    // function - read or write
    req_msg[8] = 0x1B;
    req_msg[9] = func;

    // Address
    req_msg[10] = addr >> 8;
    req_msg[11] = addr & 0x00ff;

    // Number of bytes
    req_msg[10] = count >> 8;
    req_msg[11] = count & 0x00ff;

    return 12;
}

int clMemlink::_checkresponse(
    int func)
{

}
