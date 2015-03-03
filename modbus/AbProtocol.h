/*
 * AbProtocol.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef ABPROTOCOL_H_
#define ABPROTOCOL_H_

class AbProtocol
{
public:
	AbProtocol();
	virtual ~AbProtocol();

	void read();
	void write();

	void setProtocolBehavior(AbConnect con);

private:
	AbConnect devConnect;

};

#endif /* ABPROTOCOL_H_ */
