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
#ifndef _UNI_TIMER_H
#define _UNI_TIMER_H
#include <ostream>
#include <ctime>
#include <stdint.h> //C98

#ifdef _WIN32
#include <time.h>
#include <windows.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
#endif

#ifdef __linux__
#include <sys/time.h>
#endif

namespace utils {

typedef uint64_t TimeMs; //Time in [ms]
typedef double TimeJD;  //Time in Julian days
typedef long int TimeJDN; //Julian day number

class UniTimer {
public:

    struct TimeDate {
        int msec;
        int sec;
        int min;
        int hour;
        int day;
        int month;
        int year;
    };//Time contains a date

    enum MonDefs { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
public:

    static TimeMs getCurrentTime();
    static TimeMs getCurrentTimeHR();
    static TimeMs dateToTimeMs(int year, int month, int day, int hour, int min, int sec);
    static TimeJD dateToTimeJD(int year, int month, int day, int hour, int min, int sec);

    static TimeMs createTimeInterval(int sec, int min = 0, int hour = 0, int day = 0);
    static TimeMs createTimeIntervalPrec(int msec, int sec);
    static TimeDate convertToTimeDate(const TimeMs &ms);
    static TimeDate convertToTimeDate(const TimeJD &jd);


    void startTimer();
    TimeMs getElaspedTime();

    static float getFPS();

    inline
    static TimeJDN getJDN(const TimeJD &jd) {
        return static_cast<TimeJDN>(jd + 0.5);
    }

};

typedef UniTimer::TimeDate TimeDate; //Complex date
std::ostream& operator <<(std::ostream &os,const TimeDate &time);

template <typename T>
void fixed2time(const T ang, unsigned char time[4]) {
    const static int unit = 60*60*100;
    int v = static_cast<int>(ang * unit);
    time[0] = v % 100;
    v /= 100;
    time[1] = v % 60;
    v /= 60;
    time[2] = v % 60;
    v /= 60;
    time[3] = v;
}

inline
TimeMs UniTimer::getCurrentTime() {
    return static_cast<TimeMs>( time(NULL)*1000UL );
}
inline
TimeMs UniTimer::getCurrentTimeHR() {
#if defined(_WIN32)
    FILETIME ft;
    uint64_t tmpres = 0;
    static int tzflag = 0;

    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    tmpres /= 10UL; 
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tmpres /= 1000UL; //convert to miliseconds
    return static_cast<TimeMs>( tmpres );
#elif defined(__linux__)
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t mtime = now.tv_sec;
    mtime *= 1000UL;
    mtime += ((uint64_t)now.tv_usec / 1000UL);
    return mtime;
#else
    return 0;
#endif

}
inline
TimeMs UniTimer::dateToTimeMs(int year, int month, int day, int hour, int min, int sec) {
    struct tm *dtime;
    time_t rawtime;
    time ( &rawtime );
    dtime = localtime ( &rawtime );
    dtime->tm_sec = sec;
    dtime->tm_min = min;
    dtime->tm_hour = hour;
    dtime->tm_mday = day;
    dtime->tm_mon = month - 1;
    dtime->tm_year = year - 1900;
    time_t tt = mktime(dtime) * 1000;
    return static_cast<TimeMs>(tt);
}
inline
TimeMs UniTimer::createTimeInterval(int sec, int min, int hour, int day) {
    return (sec + min * 60 + hour * 60 * 60 + day * 60 * 60 * 24) * 1000;
}
inline
TimeMs UniTimer::createTimeIntervalPrec(int msec, int sec) {
    return msec + sec * 1000;
}
inline
TimeDate UniTimer::convertToTimeDate(const TimeMs &ms) {
    int msec = ms % 1000;
    time_t tt = static_cast<time_t>(ms / 1000);
    tm *time = localtime( &tt );
    TimeDate ut = { msec, time->tm_sec, time->tm_min, time->tm_hour, time->tm_mday, time->tm_mon + 1, time->tm_year + 1900 };
    return ut;
}
inline
TimeJD UniTimer::dateToTimeJD(int year, int month, int day, int hour, int min, int sec) { 
    long long a = (14 - month)/12;
    long long y = year + 4800 - a;
    long long m = month + 12*a - 3;
    TimeJD jdn = static_cast<TimeJD>(day + (153 * m + 2)/5 + 365*y + y/4 - y/100 + y/400 - 32045);
    TimeJD jd = jdn + (hour-12.0)/24.0 + min/1440.0 + sec/86400.0;	
    return jd;
}
inline
TimeDate UniTimer::convertToTimeDate(const TimeJD &jd) {
    TimeDate dt;
    long long j = static_cast<long long>(jd + 0.5) + 32044;
    long long g = j / 146097;
    long long dg = j % 146097;
    long long c = (dg / 36524 + 1) * 3 / 4;
    long long dc = dg - c * 36524;
    long long b = dc / 1461;
    long long db = dc % 1461;
    long long a = (db / 365 + 1) *3 / 4;
    long long da = db - a * 365;
    long long y = g * 400 + c * 100 + b * 4 + a;
    long long m = (da * 5 + 308) / 153 - 2;
    long long d = da - (m+4) * 153 / 5 + 122;
    dt.year = static_cast<int>(y - 4800 + (m + 2) / 12);
    dt.month = (m + 2) % 12 + 1;
    dt.day = static_cast<int>(d + 1);

    double secjd = static_cast<double>(jd) - 0.5;
    double sectime = (secjd - static_cast<double>(static_cast<int>(secjd)))*24.0;
    unsigned char prectime[4];
    fixed2time(sectime, prectime);

    dt.msec = prectime[0];
    dt.sec = prectime[1];
    dt.min = prectime[2];
    dt.hour = prectime[3];

    return dt;
}
inline
std::ostream& operator <<(std::ostream &os,const TimeDate &time) {
    os << time.year << "-" << time.month << "-" << time.day << " " 
        << time.hour << ":" << time.min << ":" << time.sec << "." << time.msec;
    return os;
}

}//namespace utils

#endif
