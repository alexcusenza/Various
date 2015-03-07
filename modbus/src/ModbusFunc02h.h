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
	ModbusFunc02h(int, int);
	virtual ~ModbusFunc02h();

	void setAddr(int);
	void setNumBits(int);
	void setRspLength(int);

	ModbusFunc02h * message();
	void check();

private:



};

#endif /* MODBUSFUNC02H_H_ */
