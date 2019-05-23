#include "Log.h"

#include <comdef.h>


using namespace std;


Log Log::instance;


Log::Log() {
    sout.open("log.txt", fstream::out);
}


Log::~Log() {
    sout.close();
}


void Log::Write(wstring str) {
    _bstr_t bstr(str.c_str());
    const char* nstr = bstr;
    Write(nstr);
}


void Log::Write(string str) {
    Write(str.c_str());
}


void Log::Write(const char* str) {
    Log::instance.sout << endl;
    WriteRaw(str);
    Log::instance.sout << endl;
}


void Log::WriteRaw(wstring str) {
    _bstr_t bstr(str.c_str());
    const char* nstr = bstr;
    WriteRaw(nstr);
}


void Log::WriteRaw(string str) {
    WriteRaw(str.c_str());
}


void Log::WriteRaw(const char* str) {
    Log::instance.sout << str;
}
