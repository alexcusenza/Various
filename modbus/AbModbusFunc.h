/*
 * AbModbusFunc.h
 *
 *  Created on: Mar 2, 2015
 *      Author: jbw
 */

#ifndef ABMODBUSFUNC_H_
#define ABMODBUSFUNC_H_

class AbModbusFunc {
public:
	AbModbusFunc();
	virtual ~AbModbusFunc();

protected:
	void buildmessage(int, int, int, int);
	void checkresponse();
	virtual void message();
};

#endif /* ABMODBUSFUNC_H_ */
