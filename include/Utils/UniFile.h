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
#ifndef _UNI_FILE_H
#define _UNI_FILE_H

#include <fstream>
#if defined(_WIN32)
#include <codecvt>
#endif
#include <locale>
#include <wchar.h>
#include <cstring>
#include <string>
#ifdef __linux__
#include <tr1/memory>
#else
#include <memory>
#endif
#include "UniException.h"

namespace utils {

class UniFile {
public:
    enum UFMode { UF_READ, UF_WRITE };
public:
    UniFile(const std::string &fileName, UFMode fileMode);
    ~UniFile();

    bool is_open() const;
    bool eof() const;

    void get();
    void get(char &c);
    void get(wchar_t &c);

    void get(char *str, std::streamsize count);
    void get(wchar_t *str, std::streamsize count);

    void getline(char *str, std::streamsize count);
    void getline(wchar_t *str, std::streamsize count);

    char peek();
    wchar_t widepeek();

    bool is_widechar() const;

public:
    class InternalInterface;
private:
    UniFile(const UniFile &);
    UniFile &operator=(const UniFile &);
private:
    std::tr1::shared_ptr< InternalInterface > mFile;
    bool mWide;
};

inline
bool UniFile::is_widechar() const {
    return mWide;
}

#define BOM_UTF8_ID 0x00bfbbef
class UniFile::InternalInterface {
public:
//	InternalInterface() {}
    virtual bool is_open() const = 0;
    virtual bool eof() const = 0;

    virtual void get() = 0;
    virtual void get(char &c) = 0;
    virtual void get(char *str, std::streamsize count) = 0;
    virtual void getline(char *str, std::streamsize count) = 0;
    virtual void get(wchar_t &c) = 0;
    virtual void get(wchar_t *str, std::streamsize count) = 0;
    virtual void getline(wchar_t *str, std::streamsize count) = 0;
    virtual char peek() = 0;
    virtual wchar_t widepeek() = 0;
};

class Ascii_F : public UniFile::InternalInterface {
public:
    Ascii_F(const char *fname, const std::ios_base::openmode mode) {
        mFileStream.open(fname, mode);
    }
    ~Ascii_F() {
        mFileStream.close();
    }
    inline bool is_open() const { return mFileStream.is_open(); }
    inline bool eof() const { return mFileStream.eof(); }

    inline void get() { mFileStream.get(); }
    inline void get(char &c) { mFileStream.get(c); }
    inline void get(char *str, std::streamsize count) { mFileStream.get(str, count); }
    inline void getline(char *str, std::streamsize count) { mFileStream.getline(str, count); }
    inline char peek() { return mFileStream.peek(); }
    inline wchar_t widepeek() { return mFileStream.peek(); }

    void get(wchar_t &c) { 
        char ch;
        mFileStream.get(ch); 
        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t)); 
        mbrtowc(&c, &ch, 1, &state); 
    }
    void get(wchar_t *str, std::streamsize count) { 
        char *chr = new char[static_cast<unsigned int>(count)];
        mFileStream.get(chr, count);
        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t)); 
        mbrtowc(str, chr, static_cast<std::size_t>(count), &state); 
        delete [] chr;
    }
    void getline(wchar_t *str, std::streamsize count) {
        char *chr = new char[static_cast<unsigned int>(count)];
        mFileStream.getline(chr, count);
        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t)); 
        mbrtowc(str, chr, static_cast<std::size_t>(count), &state); 
        delete [] chr;
    }

private:
    std::string mStr;
    std::ifstream mFileStream;
};
class UTF8_F : public UniFile::InternalInterface {
public:
    UTF8_F(const char *fname, const std::ios_base::openmode mode) {
        mFileStream.open(fname, mode);
#if defined(_WIN32)
        const std::locale empty_locale;
        typedef std::codecvt_utf8<wchar_t> converter_type;
        const converter_type* converter = new converter_type;
        const std::locale utf8_locale = std::locale(empty_locale, converter);
        mFileStream.imbue(utf8_locale);
#endif
#if defined(__linux__)
        try {
            mFileStream.imbue(std::locale("pl_PL.UTF-8"));
        }
        catch (std::runtime_error &e) {
            throw UniException("Locale pl_PL.UTF-8 missing. ", e.what());
        }
#endif
        wchar_t wc;
        wc = mFileStream.get();
    }
    ~UTF8_F() {
        mFileStream.close();
    }
    inline bool is_open() const { return mFileStream.is_open(); }
    inline bool eof() const { return mFileStream.eof(); }

    inline void get() { mFileStream.get(); }
    void get(char &c) { 
        wchar_t ch;
        mFileStream.get(ch);
        wcstombs(&c, &ch, 1); 
    }
    void get(char *str, std::streamsize count) { 
        wchar_t *chr = new wchar_t[static_cast<unsigned int>(count)];
        mFileStream.get(chr, count);
        mbstate_t state;
        wcstombs(str, chr, static_cast<std::size_t>(count)); 
        delete [] chr;
    }
    void getline(char *str, std::streamsize count) {
        wchar_t *chr = new wchar_t[static_cast<unsigned int>(count)];
        mFileStream.getline(chr, count);
        wcstombs(str, chr, static_cast<std::size_t>(count)); 
        delete [] chr;
    }
    inline void get(wchar_t &c) { mFileStream.get(c); }
    inline void get(wchar_t *str, std::streamsize count) { mFileStream.get(str, count); }
    inline void getline(wchar_t *str, std::streamsize count) { mFileStream.getline(str, count); }
    inline char peek() { 
        char c;
        wchar_t wc = mFileStream.peek(); 
        wcstombs(&c, &wc, 1);
        return c;
    }
    inline wchar_t widepeek() { return mFileStream.peek(); }

private:
    std::wstring mStr;
    std::wifstream mFileStream;
};
inline
UniFile::UniFile(const std::string &fileName, UFMode fileMode):mWide(false) {
    std::ifstream stream(fileName.c_str(), std::ios::in | std::ios::binary);
    unsigned int identity;
    if(stream.is_open()) {
        stream.read((char *)&identity, sizeof(unsigned int));
        stream.close();
    }
    else
        throw UniException("File is missing ", fileName);

    std::ios_base::openmode mode = (fileMode == UF_READ) ? std::ios::in : std::ios::out;
    if((identity & BOM_UTF8_ID) == BOM_UTF8_ID) {
        mFile.reset( new UTF8_F( fileName.c_str(), mode ) );
        mWide = true;
    }else {
        mFile.reset( new Ascii_F( fileName.c_str(), mode) );
        mWide = false;
    }
}
inline
UniFile::~UniFile() {
}
inline
bool UniFile::is_open() const {
    return mFile->is_open();
}
inline
bool UniFile::eof() const {
    return mFile->eof();
}
inline
void UniFile::get() {
    mFile->get();
}
inline
void UniFile::get(char &c) {
    mFile->get(c);
}
inline
void UniFile::get(char *str, std::streamsize count) {
    mFile->get(str, count);
}
inline
void UniFile::get(wchar_t &c){
    mFile->get(c);
}
inline
void UniFile::get(wchar_t *str, std::streamsize count) {
    mFile->get(str, count);
}
inline
void UniFile::getline(char *str, std::streamsize count) {
    mFile->getline(str, count);
}
inline
void UniFile::getline(wchar_t *str, std::streamsize count) {
    mFile->getline(str, count);
}
inline
char UniFile::peek() { 
    return mFile->peek();
}
inline
wchar_t UniFile::widepeek() { 
    return mFile->widepeek();
}

}//namespace utils

#endif
