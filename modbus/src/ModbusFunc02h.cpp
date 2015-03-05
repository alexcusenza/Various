/*
 * ModbusFunc02h.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "ModbusFunc02h.h"

ModbusFunc02h::ModbusFunc02h() {

	numbytes = bitstobytes(numbits);
	func = MODBUS_FC_READ_DISCRETE_INPUTS;
	length = MODBUS_TCP_HEADER_READ_LENGTH;

	req_length = MODBUS_TCP_PRESET_REQ_LENGTH;
	rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH + numbytes;


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
	m_numbits = numbits;
}

void ModbusFunc02h::message()
{
	buildmessage(func, length, m_addr, m_numbits);
}
