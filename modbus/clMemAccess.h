/*
 * clMemAccess.h
 *
 *  Created on: Dec 21, 2014
 *      Author: jbw
 */

#ifndef CLMEMACCESS_H_
#define CLMEMACCESS_H_

class clMemAccess
{
public:
    clMemAccess();
    virtual ~clMemAccess();

    virtual void read(
        int,                    // address
        int,                    // number of bits
        unsigned char *) = 0;   // destination)

    virtual void write() = 0;
};

#endif /* CLMEMACCESS_H_ */
