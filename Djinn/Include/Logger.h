#pragma once

#ifdef LOGGER_EXPORTS
#define LOGGER_API _declspec(dllexport)
#else
#define LOGGER_API _declspec(dllimport)
#endif

#include <string>
#include <fstream>

namespace Log {
    void LOGGER_API Write(std::string str);
    void LOGGER_API Write(std::wstring str);
    void LOGGER_API Write(const char* str);

    class Logger {
    public:
        void Write(std::string str);
        void Write(std::wstring str);
        void Write(const char* str);
        Logger();
        Logger(const Logger& other) = delete;
        ~Logger();
    private:
        std::fstream fs;
    };
}
