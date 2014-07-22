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
#ifndef _UNI_THREAD_HPP
#define _UNI_THREAD_HPP

//The pthread_win32 is an option for windows 
//UniMutex/UniMutex was introduced because of lightweight unix systems, without boost libraries
#ifdef _WIN32
#ifdef USE_POSIX_PTHREAD
#include <pthread.h>
#else
#include <boost/thread.hpp>
#endif
#endif

#ifdef __linux__
#ifndef USE_POSIX_PTHREAD
#define USE_POSIX_PTHREAD
#endif
#include <pthread.h>
#endif

namespace utils {

typedef  void* (*vvfunction)(void*);

#ifdef USE_POSIX_PTHREAD
class UniThread {
public:
    UniThread() {
    }
    template<typename TH_ARG_THIS>
    void createNewThread(vvfunction func, TH_ARG_THIS args) {
        pthread_create(&mT, NULL, func, reinterpret_cast<void*>(args));
    }
    void join() {
        pthread_join(mT, NULL);
    }
    void interrupt() {
        pthread_cancel(mT);
    }
private:
    UniThread(UniThread &);
    UniThread &operator=(const UniThread &other);
private:
    pthread_t mT;
};

#else
class UniThread {
public:
    UniThread() {
    }
    template<typename TH_ARG_THIS>
    void createNewThread(vvfunction func, TH_ARG_THIS args) {
        mT = boost::thread(func, reinterpret_cast<void*>(args));
    }
    void join() {
        mT.join();
    }
    void interrupt() {
        mT.interrupt();
    }
private:
    UniThread(UniThread &);
    UniThread &operator=(const UniThread &other);
private:
    boost::thread mT;
};

#endif

}//namespace utils

#endif
