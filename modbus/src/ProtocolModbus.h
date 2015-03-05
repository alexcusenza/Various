/*
 * Modbus.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef PROTOCOLMODBUS_H_
#define PROTOCOLMODBUS_H_

#include "AbProtocol.h"
#include "AbConnect.h"
#include "AbModbusFunc.h"

class ModbusProtocol : public AbProtocol
{
public:
	ModbusProtocol();
	virtual ~ModbusProtocol();

	void setModReadFunc(AbModbusFunc);
	void setModWriteFunc(AbModbusFunc);

	void Read();
	void Write();

private:
	AbConnect devConnect;
	AbModbusFunc modReadFunction;
	AbModbusFunc modWriteFunction;
};

#endif /* PROTOCOLMODBUS_H_ */