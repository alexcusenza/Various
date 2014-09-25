/*
 * clTimeSpec.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: jbw
 */

#include "clTimeSpec.h"

clTimeSpec::clTimeSpec()
{
    // TODO Auto-generated constructor stub
}

clTimeSpec::~clTimeSpec()
{
    // TODO Auto-generated destructor stub
}

void clTimeSpec::GetTimePrev()
{
    timespec tmp;
    tmp =_GetTime();
    mts_y = tmp;
}

void clTimeSpec::GetTimeCurr()
{
    timespec tmp;
    tmp = _GetTime();
    mts_x = tmp;
}

void clTimeSpec::CurrToPrev()
{
    mts_y = mts_x;
}

timespec clTimeSpec::_GetTime()
{
    timespec tmp;
    clock_gettime(CLOCK_MONOTONIC, &tmp);
    return tmp;
}

double clTimeSpec::TimeDiffCombine()
{
    timespec tmp;
    tmp = TimeDiff();
    return TimeCombine(tmp);
}


double clTimeSpec::TimeCombine(timespec tmp)
{
    return (tmp.tv_sec + tmp.tv_nsec/NSEC_IN_SECS);
}


timespec clTimeSpec::TimeDiff()
{
    /* Perform the carry for the later subtraction by updating y. */
    int nsec;

    if (mts_x.tv_nsec < mts_x.tv_nsec)
    {
        nsec = (mts_y.tv_nsec - mts_x.tv_nsec) / NSEC_IN_SECS + 1;
        mts_y.tv_nsec -= NSEC_IN_SECS * nsec;
        mts_y.tv_sec += nsec;
    }
    if (mts_x.tv_nsec - mts_y.tv_nsec > NSEC_IN_SECS)
    {
        nsec = (mts_x.tv_nsec - mts_y.tv_nsec) / NSEC_IN_SECS;
        mts_y.tv_nsec += NSEC_IN_SECS * nsec;
        mts_y.tv_sec -= nsec;
    }

    m_result.tv_sec = mts_x.tv_sec - mts_y.tv_sec;
    m_result.tv_nsec = mts_x.tv_nsec - mts_y.tv_nsec;

    return m_result;
}


