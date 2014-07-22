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
#ifndef _UNI_MUTEX_HPP
#define _UNI_MUTEX_HPP

//The pthread_win32 is an option for windows
//UniMutex/UniMutex was introduced because of lightweight unix systems, without boost libraries
#ifdef _WIN32
#ifdef USE_POSIX_PTHREAD
#include <pthread.h>
#else
#include <boost/thread/mutex.hpp>
#endif
#endif

#ifdef __linux__
#ifndef USE_POSIX_PTHREAD
#define USE_POSIX_PTHREAD
#endif
#include <pthread.h>
#include <errno.h>
#endif

namespace utils {

#ifdef USE_POSIX_PTHREAD
class UniMutex {
public:
    UniMutex() {
        pthread_mutex_init(&mM, NULL);
    }
    void lock() {
        pthread_mutex_lock (&mM);
    }
    void unlock() {
        pthread_mutex_unlock(&mM);
    }
    bool trylock() {
        return (pthread_mutex_trylock (&mM) == EBUSY);
    }
    ~UniMutex() {
        pthread_mutex_destroy (&mM);
    }
private:
    UniMutex(UniMutex &);
    UniMutex &operator=(const UniMutex &other);
private:
    pthread_mutex_t mM;
};
#else
class UniMutex {
public:
    UniMutex() {}
    void lock() {
        mM.lock();
    }
    void unlock() {
        mM.unlock();
    }
    bool trylock() {
        return mM.try_lock();
    }
    ~UniMutex() {
        mM.destroy();
    }
private:
    UniMutex(UniMutex &);
    UniMutex &operator=(const UniMutex &other);
private:
    boost::mutex mM;
};
#endif

class UniScopedLock {
public:
    UniScopedLock(UniMutex &m) : mRef(m) {
        mRef.lock();
    }
    ~UniScopedLock() {
        mRef.unlock();
    }
private:
    UniScopedLock();
    UniScopedLock(UniScopedLock &);
    UniScopedLock &operator=(const UniScopedLock &other);
private:
    UniMutex &mRef;
};

}//namespace utils

#endif

