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
#include <iostream>
#include "UniCommon.h"

int main() {

    utils::UniSettings read("test.ini");
    std::string str = read.Get("Test", "name", "");
    int val = read.GetInteger("Test", "value", 100);

    {
        utils::UniFile utf8file("utf8.txt", utils::UniFile::UF_READ);
        if(utf8file.is_open()) {
            std::cout << "utf8 test ok" << std::endl;
        }
    }

    return 0;
};
