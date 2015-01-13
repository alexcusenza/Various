/*
 * clModbusTCP.h
 *
 *  Created on: Oct 22, 2014
 *      Author: jbw
 */

#ifndef CLMODBUSTCP_H_
#define CLMODBUSTCP_H_

#include "clMemAccess.h"
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

/* ============================================================================
 * Modbus Protocol
 *              BYTES
 * Transaction  - 2
 * Protocol     - 2
 * Length       - 2     -> indicate the number of bytes following
 * UID          - 1
 * Function     - 1
 * Address      - 2
 * Size         - 2
 * Size         - 1     -> for writes only
 * ============================================================================
 */

#define MODBUS_TCP_HEADER_READ_LENGTH       6
// 6 bytes =  uid-1, func-1, addr-2, bits-2

#define MODBUS_TCP_HEADER_WRITE_LENGTH      7
// 7 bytes =  uid-1, func-1, addr-2, bits/bytes-2, bytes/words-1

#define MODBUS_TCP_PRESET_REQ_LENGTH        12
// 12 bytes = transid-2, protoid-2, length-2, uid-1, func-1, addr-2, size-2

#define MODBUS_TCP_PRESET_RSP_LENGTH        12
// 12 bytes = transid-2, protoid-2, length-2, uid-1, func-1, addr-2, size-2

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



class clModbusTCP : public clMemAccess
{
public:
    clModbusTCP(int,  const char *);
    virtual ~clModbusTCP();

    void read();
    void write();

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
    void _buildmessage(
        int,                // function
        int,                // length
        int,                // address
        int);                // number of bytes

    int _checkresponse(
        const int );         // function

    clSocket * mp_socket;
    int m_uid;

    unsigned char * req_msg;
    int req_length;
    unsigned char * rsp_msg;
    int rsp_length;

};

#endif /* CLMODBUSTCP_H_ */
