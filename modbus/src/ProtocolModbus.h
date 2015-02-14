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
	ModbusProtocol(AbConnect, AbModbusFunc, AbModbusFunc);
	virtual ~ModbusProtocol();

	void setConnectDevice(AbConnect);
	void setModReadFunc(AbModbusFunc);
	void setModWriteFunc(AbModbusFunc);

	void Read();
	void Write();

private:
	AbConnect devConnect;
	AbModbusFunc modRead;
	AbModbusFunc modWrite;
};

#endif /* PROTOCOLMODBUS_H_ */
