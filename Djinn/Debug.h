#pragma once

#include <exception>
#include <comdef.h>
#include <DbgHelp.h>

#include "Log.h"


#define STRINGIFY(x) #x


namespace Djn
{
    class Symbol {
    public:
        Symbol(DWORD maxNameLength) : maxNameLength(maxNameLength) {
            size_t symbolSize = sizeof(IMAGEHLP_SYMBOL64) + maxNameLength;
            this->imageHlpSymbol64 = (IMAGEHLP_SYMBOL64*)malloc(symbolSize);
            this->imageHlpSymbol64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            this->imageHlpSymbol64->MaxNameLength = maxNameLength;
        }
        ~Symbol() {
            free(this->imageHlpSymbol64);
        }
        DWORD GetMaxNameLength() const { return this->maxNameLength; }
        IMAGEHLP_SYMBOL64* Get() const { return imageHlpSymbol64; }
    private:
        DWORD maxNameLength;
        IMAGEHLP_SYMBOL64 * imageHlpSymbol64;
    };

    /// Adds a stack trace to exception throwing.
    class Exception : public std::exception
    {
    public:
        Exception(const char* what) : exception(what)
        {
            try {
                int maxFilePathLength = 0;
                int maxFileNameLength = 0;
                int maxDigitCount = 0;
                GetStackTrace(maxFilePathLength, maxFileNameLength, maxDigitCount, 2);
                maxFilePathLength += 2;
                maxFileNameLength += 2;
                maxDigitCount += 2;
                stackTrace += "Stack Trace:";
                stackTrace += GetStackTrace(maxFilePathLength, maxFileNameLength, maxDigitCount, 2);
            }
            catch (std::exception e) {
                Log::Write(std::string("Failed to retrieve stack trace: ") + e.what());
                throw std::exception(what);
            }
        }
        Exception(std::string what) : Exception(what.c_str()) {}
        Exception(std::wstring what) : Exception(_bstr_t(what.c_str())) {}
        ~Exception() {}
        std::string StackTrace() const
        {
            return stackTrace;
        }
    private:
        std::string stackTrace;
        std::string GetStackTrace(
            int& maxFilePathLength,
            int& maxFileNameLength,
            int& maxDigitCount,
            int skipLineCount)
        {
            std::string traceString;
            std::string formatStr =
                std::string("\n%-") + std::to_string(maxFilePathLength) +
                std::string("s%") + std::to_string(maxFileNameLength) +
                std::string("s:%-") + std::to_string(maxDigitCount) +
                std::string("d %s");

            // Prepare to get the stack trace.
            auto hCurrentProcess = GetCurrentProcess();
            SymInitialize(hCurrentProcess, NULL, true);
            auto hCurrentThread = GetCurrentThread();

            CONTEXT context = {};
            context.ContextFlags = CONTEXT_ALL;
            RtlCaptureContext(&context);

            STACKFRAME64 stackFrame = {};
            stackFrame.AddrPC.Mode = AddrModeFlat;
            stackFrame.AddrFrame.Mode = AddrModeFlat;
            stackFrame.AddrStack.Mode = AddrModeFlat;
#if defined(_M_IX86)
            stackFrame.AddrPC.Offset = context.Eip;
            stackFrame.AddrFrame.Offset = context.Ebp;
            stackFrame.AddrStack.Offset = context.Esp;
            auto imageFileMachine = IMAGE_FILE_MACHINE_I386;
#elif defined(_AMD64_)
            stackFrame.AddrPC.Offset = context.Rip;
            stackFrame.AddrFrame.Offset = context.Rbp;
            stackFrame.AddrStack.Offset = context.Rsp;
            auto imageFileMachine = IMAGE_FILE_MACHINE_AMD64;
#endif

            Symbol symbol(64);
            IMAGEHLP_LINE64 line = {};
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            // Get the stack trace.
            do {
                if (!StackWalk64(
                    imageFileMachine, hCurrentProcess, hCurrentThread,
                    &stackFrame, &context,
                    readMemProc,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    NULL)) {
                    throw std::exception(
                        "Unable to walk stack "
                        "while constructing Djn::Exception.");
                }

                // Get Symbol Info.
                DWORD64 symbolOffset;
                if (!SymGetSymFromAddr64(
                    hCurrentProcess,
                    stackFrame.AddrPC.Offset,
                    &symbolOffset,
                    symbol.Get())) {
                    throw std::exception(
                        "Unable to retrieve symbol from address "
                        "when constructing Djn::Exception.");
                }

                // TODO: continue here if this frame is related to constructing
                // this class.

                // Get Line info.
                DWORD lineOffset;
                if (!SymGetLineFromAddr64(
                    hCurrentProcess,
                    stackFrame.AddrPC.Offset,
                    &lineOffset,
                    &line)) {
                    break;
                }

                // Split full file path into path and filename.
                auto fnamePtr = line.FileName;
                auto i = 0;
                auto lastIdxOfSep = 0;
                for (;fnamePtr[i] != '\0'; ++i) {
                    if (fnamePtr[i] == '\\') lastIdxOfSep = i;
                }
                fnamePtr = line.FileName + lastIdxOfSep + 1;
                // There's gotta be a better way to do this.
                // But it works for now.
                auto fpathPtr = (char*)malloc(lastIdxOfSep + 2);
                memcpy(fpathPtr, line.FileName, lastIdxOfSep + 1);
                fpathPtr[lastIdxOfSep + 1] = '\0';

                // Resize the name lengths.
                if (lastIdxOfSep > maxFilePathLength)
                    maxFilePathLength = lastIdxOfSep;
                if (i - lastIdxOfSep > maxFileNameLength)
                    maxFileNameLength = i - lastIdxOfSep;
                auto lineNumTmp = line.LineNumber;
                auto digitCount = 0;
                while (lineNumTmp) {
                    digitCount++;
                    lineNumTmp /= 10;
                }
                if (digitCount > maxDigitCount)
                    maxDigitCount = digitCount;

                // Construct pretty string.
                std::string frameString;
                char* buffer = new char[255];
                sprintf_s(
                    buffer, 255, formatStr.c_str(),
                    fpathPtr, fnamePtr, line.LineNumber, symbol.Get()->Name);
                frameString = buffer;
                traceString += frameString;
                delete(buffer);
                free(fpathPtr);
            } while (stackFrame.AddrReturn.Offset != 0);

            return traceString;
        }
        static BOOL __stdcall readMemProc(
            HANDLE hProcess,
            DWORD64 qwBaseAddress,
            PVOID lpBuffer,
            DWORD nSize,
            LPDWORD lpNumberOfBytesRead)
        {
            SIZE_T st;
            BOOL bRet = ReadProcessMemory(
                hProcess, (LPVOID)qwBaseAddress, lpBuffer, nSize, &st);
            *lpNumberOfBytesRead = (DWORD)st;
            return bRet;
        }
    };
}
