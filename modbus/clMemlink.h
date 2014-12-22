/*
 * clMemlink.h
 *
 *  Created on: Dec 19, 2014
 *      Author: jbw
 */

#ifndef CLMEMLINK_H_
#define CLMEMLINK_H_

#include "clSocket.h"

class clMemlink
{
public:
    clMemlink(int, const char *);
    virtual ~clMemlink();

    int read(
        int,                // address
        int,                // num words
        unsigned char *);   // destination

    int write(
        int,                // address
        int,                // num words
        unsigned char *);   // destination

private:
    int _buildmessage(
        int,                // function
        int,                // length
        int,                // address
        int);               // num words

    int _checkresponse(
        const int);         // function


    clSocket * mp_socket;

    unsigned int * req_msg;
    int req_length;
    unsigned int * rsp_msg;
    int rsp_length;

};

#endif /* CLMEMLINK_H_ */
