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
#ifndef _UNI_EXCEPTION_H
#define _UNI_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace utils {

struct UniException : public std::runtime_error {
public:
    UniException(const std::string &what, const std::string &adds = "") : std::runtime_error(std::string("Connection exception:") + what + adds) {}
    ~UniException(void) throw() {}
};

}//namespace utils

#endif
