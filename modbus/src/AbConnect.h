/*
 * AbConnect.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef ABCONNECT_H_
#define ABCONNECT_H_

class AbConnect {
public:
	AbConnect();
	virtual ~AbConnect();

	virtual void Send();
	virtual void Recv();
};

#endif /* ABCONNECT_H_ */
