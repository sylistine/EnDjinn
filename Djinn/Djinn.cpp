#include <Windows.h>
#include <Windowsx.h>
#include <iostream>

#include "Djinn.h"
#include "App.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR args,
    int showflag)
{
    // Pretty much just asserting there is no logger exception.
    try {
        Log::WriteRaw("EnDjinn v3.3\n");
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    // Running the actual app.
    try {
        Djn::App app(hInstance);
        app.Run();
    } catch (Djn::Exception e) {
        Log::Write(std::string("Djinn Exception caught --------------------------------"));
        Log::Write(e.what());
        Log::Write(e.StackTrace());
        Log::Write("-------------------------------------------------------");
    } catch (std::exception e) {
        Log::Write(std::string("Standard Exception caught --------------------------------"));
        Log::Write(e.what());
        Log::Write("-------------------------------------------------------");
    }

    Log::Write("Closing Djinn.");
    return 0;
}
