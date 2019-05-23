#pragma once

#include "Window.h"
#include "Time.h"
#include "Gfx.h"

namespace Djn
{
    class App
    {
    private:
        static App* instance;
    public:
        App(HINSTANCE hInstance);
        ~App();
        void Run();
    private:
        Action<> resizeWindowAction;
        Action<> pauseAction;
        Action<> unpauseAction;
        Action<> quitAction;

        Timer timer;
        MainWindow mainWindow;
        bool paused;
        bool quitting = false;

        void ResizeWindow();
        void Pause();
        void Unpause();
        void SetQuitFlag();
    };
}
