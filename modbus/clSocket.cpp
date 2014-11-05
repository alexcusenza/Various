/*
 * clSocket.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#include "clSocket.h"

clSocket::clSocket(
    const char * ipaddr):
    m_ipaddr(ipaddr)
{


}

clSocket::~clSocket()
{
    // TODO Auto-generated destructor stub
}

int clSocket::Connect()
{
    struct sockaddr_in socketname;
    int ret;

    _disconnect();
    if (socketid != 0)
    {
        close(socketid);
        socketid = 0;
    }

    socketid = socket(AF_INET, SOCK_STREAM, 0);

    if (socketid < 0)
        return -1;
    else
    {
        memset(&socketname, 0, sizeof(socketname));
        socketname.sin_family = AF_INET;
        socketname.sin_port = htons(PORT);
        socketname.sin_addr.s_addr = inet_addr(m_ipaddr);

        // Set the socket in non-blocking mode
        fcntl(socketid, FNONBLOCK, FNDELAY);

        ret = connect(socketid, (struct sockaddr *)&socketname, sizeof(socketname));

        if (ret < 0)
        {
            _disconnect();
            printf("socket connect error - errno: %d\n", errno);
            return -1;
        }
        else
            printf("socket connected");

    }

    return 0;
}

int clSocket::_disconnect()
{
    if (socketid != -1)
    {
        close(socketid);
    }
    socketid = -1;
    return 0;
}

int clSocket::_isconnected()
{
    if (socketid == -1)
        return 0;

    return 1;
}


int clSocket::SendMessage(unsigned char *message, int length)
{
    if (_isconnected() == 0)
        return -1;

    error = send(socketid, message, length, 0);

    return error;
}

int clSocket::ReceiveMessage(unsigned char *message, int length)
{
    int timeout = 1000;     // ms

    if (_isconnected() == 0)
        return -1;

    error = _select(timeout);

    if (error > 0)
        error = recv(socketid, message, length, 0);


    return error;
}

int clSocket::_select(int timeout)
{
    struct timeval stimeout;
    fd_set rset;

    FD_ZERO(&rset);
    FD_SET(socketid, &rset);

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = (timeout % 1000) * 1000;

    int ret = select(socketid + 1, &rset, NULL, NULL, &stimeout);

    if (ret == -1)
    {
        printf("select error");
        return -1;
    }
    else if (ret == 0)
    {
        printf("timed out");
        return -1;
    }

    return ret;
}
