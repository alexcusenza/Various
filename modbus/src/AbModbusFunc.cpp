/*
 * AbModbusFunc.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "AbModbusFunc.h"

AbModbusFunc::AbModbusFunc() {
	// TODO Auto-generated constructor stub

}

AbModbusFunc::~AbModbusFunc() {
	// TODO Auto-generated destructor stub
}

void AbModbusFunc::buildmessage(int function, int length, int address, int count)
{
	// Transaction ID
	req_msg[0] = 0;
	req_msg[1] = 0;

	// Protocol
	req_msg[2] = 0;
	req_msg[3] = 0;

	// Length
	req_msg[4] = length >> 8;
	req_msg[5] = length & 0x00ff;

	// Unit
	req_msg[6] = (unsigned char) uid;

	// Function
	req_msg[7] = (unsigned char) function;

	// Address
	req_msg[8] = address >> 8;
	req_msg[9] = address & 0x00ff;

	// Number of bytes
	req_msg[10] = count >> 8;
	req_msg[11] = count & 0x00ff;

}
