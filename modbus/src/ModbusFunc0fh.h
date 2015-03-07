/*
 * ModbusFunc0fh.h
 *
 *  Created on: Mar 3, 2015
 *      Author: jbw
 */

#ifndef MODBUSFUNC0FH_H_
#define MODBUSFUNC0FH_H_

#include "AbModbusFunc.h"

class ModbusFunc0fh : AbModbusFunc
{
public:
	ModbusFunc0fh();
	virtual ~ModbusFunc0fh();

	void setAddr(int);
	void setNumBits(int);

	void message();
	void check();

private:
	int m_addr;
	int m_numbits;

	int retval;
	int bytecount;
	const int func;
	int length;
	int numbytes;

	unsigned char * req_length;
	unsigned char * rsp_length ;

};

#endif /* MODBUSFUNC0FH_H_ */
