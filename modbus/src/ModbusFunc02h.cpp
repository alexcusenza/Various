/*
 * ModbusFunc02h.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "ModbusFunc02h.h"

ModbusFunc02h::ModbusFunc02h(int uid, int num)
{
	m_func = MODBUS_FC_READ_DISCRETE_INPUTS;
	m_length = MODBUS_TCP_HEADER_READ_LENGTH;
	req_length = MODBUS_TCP_PRESET_REQ_LENGTH;
	setAddr(addr);
	setRspLength(num);
}

ModbusFunc02h::~ModbusFunc02h() {
	// TODO Auto-generated destructor stub
}

void ModbusFunc02h::setAddr(int addr)
{
	m_addr = addr;
}

void ModbusFunc02h::setNumBits(int numbits)
{
	m_count = numbits;

	setRspLength(numbits);
}

void ModbusFunc02h::setRspLength(int num)
{
	rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH + bitstobytes(num);
}

ModbusFunc02h * ModbusFunc02h::message()
{
	buildmessage();

	return this;
}

void ModbusFunc02h::check()
{

}

