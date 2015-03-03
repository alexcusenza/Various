/*
 * ModbusFunc0fh.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jbw
 */

#include "ModbusFunc0fh.h"

ModbusFunc0fh::ModbusFunc0fh() {
	// TODO Auto-generated constructor stub
	numbytes = bitstobytes(numbits);
	func = MODBUS_FC_READ_DISCRETE_INPUTS;
	length = MODBUS_TCP_HEADER_READ_LENGTH;

	req_length = MODBUS_TCP_PRESET_REQ_LENGTH;
	rsp_length = MODBUS_TCP_PRESET_RSP_LENGTH + numbytes;
}

ModbusFunc0fh::~ModbusFunc0fh() {
	// TODO Auto-generated destructor stub
}

void ModbusFunc0fh::setAddr(int addr)
{


}


void ModbusFunc0fh::setNumBits(int numbits)
{
	numbytes = bitstobytes(numbits);
}

void ModbusFunc0fh::message()
{
	buildmessage(func, length, m_addr, m_numbits);

	req_msg[req_length++] = numbytes;

	// Add data to message
	for (int i = 0; i < numbytes; i++)
	{
		req_msg[req_length++] = src[i];
	}
}
