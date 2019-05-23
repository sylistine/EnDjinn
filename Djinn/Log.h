
#pragma once

#include <string>
#include <fstream>

class Log {
public:
    static void Write(std::wstring str);
    static void Write(std::string str);
    static void Write(const char* str);
    static void WriteRaw(std::wstring str);
    static void WriteRaw(std::string str);
    static void WriteRaw(const char* str);
private:
    static Log instance;
    Log();
    Log(const Log& other) = delete;
    ~Log();
    std::fstream sout;
};
