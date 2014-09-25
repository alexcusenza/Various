/*
 * clTimeSpec.h
 *
 *  Created on: Sep 5, 2014
 *      Author: jbw
 */

#ifndef CLTIMESPEC_H_
#define CLTIMESPEC_H_

#include <time.h>

static const float NSEC_IN_SECS = 1000000000.0;

class clTimeSpec
{
public:
    clTimeSpec();
    virtual ~clTimeSpec();

    void GetTimePrev();
    void GetTimeCurr();
    void CurrToPrev();

    timespec TimeDiff();
    double TimeDiffCombine();
    double TimeCombine(timespec);

    const timespec& getTxClk() const
    {
        return mts_x;
    }

    const timespec& getTyClk() const
    {
        return mts_y;
    }

    const timespec& getResult() const
    {
        return m_result;
    }

private:
    timespec _GetTime();

    timespec mts_y;
    timespec mts_x;
    timespec m_result;

};

#endif /* CLTIMESPEC_H_ */
