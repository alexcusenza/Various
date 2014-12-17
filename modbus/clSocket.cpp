/*
 * clSocket.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#include "clSocket.h"

clSocket::clSocket(
    const char * ipAddr):
    m_ipaddr(ipAddr)
{
    Connect();
    //memset(struct sockaddr,0,sizeof(sockaddr));
}

clSocket::~clSocket()
{
    _disconnect();
}

int clSocket::_disconnect()
{
    if (socketid != -1)
    {
        close(socketid);
    }
    socketid = -1;

    printf("Socket Disconnected: %d \n", socketid);
    return 1;
}

int clSocket::_isconnected()
{
    if (socketid == -1)
    {
        printf("socketid = -1 \n");
        return -1;
    }

    return 1;
}

int clSocket::_select(int timeout)
{
    int retval;
    struct timeval stimeout;
    fd_set rset;

    FD_ZERO(&rset);
    FD_SET(socketid, &rset);

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = (timeout % 1000) * 1000;

    retval = select(socketid + 1, &rset, NULL, NULL, &stimeout);
    if (retval < 0)
    {
        printf("Select Function Error \n");
        return -1;
    }
    else if (retval == 0)
    {
        printf("Select Function TimeOut \n");
        return -1;
    }
    printf("Select Function return: %d \n", retval);

    return 1;
}

int clSocket::Connect()
{
    struct sockaddr_in socketname;
    int ret;

    _disconnect();

    socketid = socket(AF_INET, SOCK_STREAM, 0);
    if (socketid < 0)
    {
        perror("Socket command error ");
        return -1;
    }
    printf("Socket Established: Socketid =  %d \n", socketid);

    //int value = 1;
    //setsockopt(socketid, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    memset(&socketname, 0, sizeof(socketname));
    socketname.sin_family = AF_INET;
    socketname.sin_port = htons(PORT);
    socketname.sin_addr.s_addr = inet_addr(m_ipaddr);

    // Set the socket in non-blocking mode
    //fcntl(socketid, FNONBLOCK, FNDELAY);

    ret = connect(socketid, (struct sockaddr *)&socketname, sizeof(socketname));
    if (ret < 0)
    {
        perror("Socket Connect Error ");
        _disconnect();
        return -1;
    }

    return 0;
}

int clSocket::SendMessage(unsigned char *message, int length)
{
    int retval;

    if (_isconnected() < 0)
        return -1;

    retval = send(socketid, message, length, 0);
    if (retval < 0)
    {
        perror("Send Error ");
        return -1;
    }

    return 1;
}

int clSocket::ReceiveMessage(unsigned char *message, int length)
{
    int retval;
    int timeout = 500;     // ms

    if (_isconnected() < 0)
        return -1;

    retval = _select(timeout);
    if (retval < 0)
        return -1;

    retval = recv(socketid, message, length, 0);
    if (retval < 0)
    {
        perror("Receive Error ");
        return -1;
    }
    else if (retval == 0)
    {
        perror("Receive Error:  Zero Bytes ");
        return -1;
    }

    return 1;
}


