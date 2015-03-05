/*
 * Socket.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef CONNECTSOCKET_H_
#define CONNECTSOCKET_H_

#include "AbConnect.h"

class SocketConnect : public AbConnect
{
public:
	SocketConnect();
	virtual ~SocketConnect();

	int SendMessage();
	int RecvMessage();
};

#endif /* CONNECTSOCKET_H_ */
