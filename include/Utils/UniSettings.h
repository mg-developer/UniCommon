//UniSettings module - rewriten from inih -- simple .INI file parser
/* 
Module Is released under the New BSD license (see LICENSE.txt). 
Go to the source project home page for more info: http://code.google.com/p/inih/
*/

#ifndef _UNI_SETTINGS_H
#define _UNI_SETTINGS_H

#include <map>
#include <string>
#include <algorithm>
#include "CReaderImplement.h"

using std::string;

namespace utils {

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class UniSettings {
public:

UniSettings(string filename) {
    _error = priv::CReaderImplement::ini_parse(filename.c_str(), ValueHandler, this);
}

int ParseError() {
    return _error;
}

string Get(string section, string name, string default_value) {
    string key = MakeKey(section, name);
    return _values.count(key) ? _values[key] : default_value;
}

long GetInteger(string section, string name, long default_value) {
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

bool GetBoolean(string section, string name, bool default_value) {
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

static string MakeKey(string section, string name) {
    string key = section + "." + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

static int ValueHandler(void* user, const char* section, const char* name,
                            const char* value) {
    UniSettings* reader = (UniSettings*)user;
    string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;
    return 1;
}

private:
    int _error;
    std::map<std::string, std::string> _values;
};


}//namespace utils

#endif

