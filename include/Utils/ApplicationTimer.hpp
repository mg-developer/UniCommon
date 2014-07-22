// Copyright 2014  Michal Gluszek <mos.gluszek@gmail.com>
//
//  This file is part of UniCommon.
//
//  UniCommon is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  UniCommon is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with UniCommon.  If not, see <http://www.gnu.org/licenses/>.
#ifndef _APPLICATION_TIMER_H
#define _APPLICATION_TIMER_H

#include <algorithm>
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace utils {

template<typename T>
void sleep(T miliseconds) {
#if defined(_WIN32)
    Sleep((unsigned int)miliseconds);
#elif defined(__linux__)
    usleep((unsigned int)miliseconds * 1000);
#endif
}

enum TimerType { CPU_TIMER = 0, REAL_TIMER = 1 };

namespace timer_detail {

template<int TYPE = utils::CPU_TIMER>
struct InternalImp {
    static double getTime(const double freq)  {
#if defined(_WIN32)
        LARGE_INTEGER query_ticks;
        QueryPerformanceCounter(&query_ticks);
        return query_ticks.QuadPart/freq;
#elif defined(__linux__)
        (void)freq;
        timespec ts;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
        return double(ts.tv_sec) + 1e-9 * double(ts.tv_nsec);
#endif
    }
};

template<>
struct InternalImp<utils::REAL_TIMER> {
    static double getTime(const double freq) {
        (void)freq;
#if defined(_WIN32)
        SYSTEMTIME st;
        GetSystemTime(&st);
        return (double)st.wSecond + 1.e-3 * (double)st.wMilliseconds;
#elif defined(__linux__)
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return double(ts.tv_sec) + 1e-9 * double(ts.tv_nsec);
#endif
    }
};

}//namespace timer_detail


template<int TYPE>
class ApplicationTimer {

    template<int TT>
    double getTime() const {
#if defined(_WIN32)
        return timer_detail::InternalImp<TT>::getTime(mFrequency);
#else
        return timer_detail::InternalImp<TT>::getTime(0.0);
#endif
    }

public:
    ApplicationTimer() {
#if defined(_WIN32)
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        mFrequency = (double)freq.QuadPart;
#endif
        reset();
    }
    ~ApplicationTimer() {}

    void reset() {
        mBests = (1e9);
        mTotals = 0;
    }
    void start() {
        mStarts = getTime<TYPE>();
    }
    void stop() {
        using namespace std;
        mTimes = getTime<TYPE>() - mStarts;
        mBests = min(mBests,mTimes);
        mTotals += mTimes;
    }
    double relative() const {
        return getTime<TYPE>() - mStarts;
    }
    double value() const {
        return mTimes;
    }
    double best() const {
        return mBests;
    }
    double total() const {
        return mTotals;
    }

private:
#if defined(_WIN32)
    double mFrequency;
#endif
    double mStarts;
    double mTimes;
    double mBests;
    double mTotals;

};

}//namespace utils

#endif
