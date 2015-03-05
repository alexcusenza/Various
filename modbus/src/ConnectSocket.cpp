/*
 * Socket.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "ConnectSocket.h"

SocketConnect::SocketConnect() {
	// TODO Auto-generated constructor stub

}

SocketConnect::~SocketConnect() {
	// TODO Auto-generated destructor stub
}

int SocketConnect::SendMessage()
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
