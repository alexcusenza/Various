/*
 * Memlink.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef MEMLINK_H_
#define MEMLINK_H_

#include "AbProtocol.h"

class MemlinkProtocol : public AbProtocol
{
public:
	MemlinkProtocol();
	virtual ~MemlinkProtocol();
};

#endif /* MEMLINK_H_ */
