/*
 * ModbusFunc02h.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef MODBUSFUNC02H_H_
#define MODBUSFUNC02H_H_

#include "AbModbusFunc.h"

class ModbusFunc02h : AbModbusFunc
{
public:
	ModbusFunc02h();
	virtual ~ModbusFunc02h();

	void setAddr(int);
	void setNumBits(int);

	void message();

private:

	int m_addr;
	int m_numbits;

	int retval;
	int bytecount;
	const int func;
	int length;
	int numbytes;


};

#endif /* MODBUSFUNC02H_H_ */
