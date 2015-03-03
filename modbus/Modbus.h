/*
 * Modbus.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include "AbProtocol.h"

class ModbusProtocol : public AbProtocol
{
public:
	ModbusProtocol();
	virtual ~ModbusProtocol();

	void setModReadFunc();

	void setModWriteFunc();

	void Read();

	void Write();

private:
	AbConnect devConnect;
};

#endif /* MODBUS_H_ */
