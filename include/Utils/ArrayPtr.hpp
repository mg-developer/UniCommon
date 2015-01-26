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
#ifndef _ARRAY_PTR_HPP
#define _ARRAY_PTR_HPP
#ifdef __linux__
#include <tr1/memory>
#else
#include <memory>
#endif

namespace utils {

template<typename T>
struct ArrayDeleter {
   void operator()(T* p) {
      delete [] p;
   }
};

template<typename T>
struct ArrayPtr : public std::tr1::shared_ptr<T> {
    ArrayPtr() :
        std::tr1::shared_ptr<T>()
    {}
    ArrayPtr(T *memory) :
        std::tr1::shared_ptr<T>(memory, ArrayDeleter<T>())
    {}
};

}//namespace utils

#endif
