/*
 * AbsDevice.h
 *
 *  Created on: Feb 4, 2015
 *      Author: jbw
 */

#ifndef ABSDEVICE_H_
#define ABSDEVICE_H_

class AbsDevice {
public:
	AbsDevice();
	virtual ~AbsDevice();
};

	virtual int read_02h();
	virtual int read_03h();
	virtual int rea

#endif /* ABSDEVICE_H_ */
