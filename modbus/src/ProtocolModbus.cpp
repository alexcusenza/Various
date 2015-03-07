/*
 * Modbus.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "ProtocolModbus.h"

ModbusProtocol::ModbusProtocol(
		AbConnect newConnect,
		AbModbusFunc newReadMod,
		AbModbusFunc newWriteMod)
:devConnect(newConnect),
 modReadFunction(newReadMod),
 modWriteFunction(newWriteMod)
{

}

ModbusProtocol::~ModbusProtocol()
{
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
	//modReadFunction.message();
	devConnect.Send(
			modReadFunction.message().req_msg,
			modReadFunction.req_length);

	devConnect.Recv();
}

void ModbusProtocol::Write()
{
	modWriteFunction.message();
	devConnect.Send(modWriteFunction.message().req_msg);
	devConnect.Recv();
}
