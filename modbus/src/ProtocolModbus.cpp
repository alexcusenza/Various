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
 modRead(newReadMod),
 modWrite(newWriteMod)
{}

ModbusProtocol::~ModbusProtocol()
{
	// TODO Auto-generated destructor stub
}


void ModbusProtocol::setConnectDevice(AbConnect newConnect)
{
	devConnect = newConnect;
}

void ModbusProtocol::setModReadFunc(AbModbusFunc readfunc)
{
	modRead = readfunc;
}

void ModbusProtocol::setModWriteFunc(AbModbusFunc writefunc)
{
	modWrite = writefunc;
}


void ModbusProtocol::Read( )
{
	devConnect.Send(
			modRead.message().req_msg,
			modRead.req_length);

	devConnect.Recv(
			modRead.rsp_msg,
			modRead.rsp_length);
}

void ModbusProtocol::Write()
{
	devConnect.Send(
			modWrite.message().req_msg,
			modWrite.req_length);

	devConnect.Recv(
			modWrite.rsp_msg,
			modWrite.rsp_length);
}
