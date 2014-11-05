/*
 * clModbusTCP.h
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#ifndef CLMODBUSTCP_H_
#define CLMODBUSTCP_H_

#include "clSocket.h"


#define MODBUS_FC_READ_COILS                0x01
#define MODBUS_FC_READ_DISCRETE_INPUTS      0x02
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_READ_INPUT_REGISTERS      0x04
#define MODBUS_FC_WRITE_SINGLE_COIL         0x05
#define MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define MODBUS_FC_READ_EXCEPTION_STATUS     0x07
#define MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
#define MODBUS_FC_REPORT_SLAVE_ID           0x11
#define MODBUS_FC_MASK_WRITE_REGISTER       0x16
#define MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17

#define MODBUS_TCP_HEADER_LENGTH      7
#define MODBUS_TCP_PRESET_REQ_LENGTH 12
#define MODBUS_TCP_PRESET_RSP_LENGTH  8

#define MIN_REQ_LENGTH 12
#define MAX_RSP_LENGTH 260


/* Modbus_Application_Protocol_V1_1b.pdf
 * (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 *
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf
 * (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 *
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 *
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_WR_WRITE_REGISTERS      121
#define MODBUS_MAX_WR_READ_REGISTERS       125



class clModbusTCP
{
public:
    clModbusTCP(int,  const char *);
    virtual ~clModbusTCP();

    int read_02h(
        int,                // address
        int,                // number of bits
        unsigned char *);   // destination

    int read_03h(
        int,                // address
        int,                // number of words
        unsigned char *);   // destination

    int write_0fh(
        int,                // address
        int,                // number of bits
        unsigned char *);   // destination

    int write_10h(
        int,                // address
        int,                // number of words
        unsigned char *);   // destination

private:
    int _buildmessage(
        int,                // function
        int,                // length
        int,                // address
        int,                // number of bytes
        unsigned char *);   // destination

    int _checkresponse(
        const int,          // function
        unsigned char *);   // response message

    clSocket * mp_socket;
    int m_uid;
    const char * m_ip;

    int ret;
    unsigned char * req_msg;
    int req_length;
    unsigned char * rsp_msg;
    int rsp_length;


};

#endif /* CLMODBUSTCP_H_ */
