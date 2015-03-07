/*
 * AbModbusFunc.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#include "AbModbusFunc.h"

AbModbusFunc::AbModbusFunc(
		int uid,
		int len,
		int addr)
: m_uid(uid),
  m_length(len),
  m_addr(addr)
{
	// TODO Auto-generated constructor stub

}

AbModbusFunc::~AbModbusFunc() {
	// TODO Auto-generated destructor stub
}

void AbModbusFunc::buildmessage()
{
	// Transaction ID
	req_msg[0] = 0;
	req_msg[1] = 0;

	// Protocol
	req_msg[2] = 0;
	req_msg[3] = 0;

	// Length
	req_msg[4] = m_length >> 8;
	req_msg[5] = m_length & 0x00ff;

	// Unit
	req_msg[6] = (unsigned char) m_uid;

	// Function
	req_msg[7] = (unsigned char) m_func;

	// Address
	req_msg[8] = m_addr >> 8;
	req_msg[9] = m_addr & 0x00ff;

	// Number of bytes
	req_msg[10] = m_count >> 8;
	req_msg[11] = m_count & 0x00ff;

}

void AbModbusFunc::checkresponse()
{

}
