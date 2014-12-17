/*
 * clSocket.h
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#ifndef CLSOCKET_H_
#define CLSOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 502

class clSocket
{
public:
    clSocket(const char *);
    virtual ~clSocket();

    int Connect();
    int Close();

    int SendMessage(
        unsigned char *,
        int);

    int ReceiveMessage(
        unsigned char *,
        int);

private:
    int _select(int);
    int _isconnected();
    int _disconnect();

    int error;
    int socketid;
    const char * m_ipaddr;

};

#endif /* CLSOCKET_H_ */
