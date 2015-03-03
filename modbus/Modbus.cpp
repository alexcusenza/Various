/*
 * Modbus.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "Modbus.h"

ModbusProtocol::ModbusProtocol() {
	// TODO Auto-generated constructor stub

}

ModbusProtocol::~ModbusProtocol() {
	// TODO Auto-generated destructor stub
}

void ModbusProtocol::setModReadFunc(AbModbusFunc readfunc)
{
	modReadFunction = readfunc;
}

void ModbusProtocol::setModWriteFunc(AbModbusFunc writefunc)
{
	modWriteFunction = writefunc;
}


void ModbusProtocol::Read()
{
	modReadFunction.message();
	devConnect.Send();
	devConnect.Recv();
}

void ModbusProtocol::Write()
{
	modWriteFunction.message();
	devConnect.Send();
	devConnect.Recv();
}
