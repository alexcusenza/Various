/*
 * Socket.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "AbConnect.h"

class SocketConnect : public AbConnect
{
public:
	SocketConnect();
	virtual ~SocketConnect();
};

#endif /* SOCKET_H_ */
