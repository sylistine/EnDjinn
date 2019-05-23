#include "App.h"


#include <functional>


using namespace std;
using namespace Djn;

App* App::instance = nullptr;

App::App(HINSTANCE hInstance) :
    resizeWindowAction(bind(&App::ResizeWindow, this)),
    unpauseAction(bind(&App::Unpause, this)),
    pauseAction(bind(&App::Pause, this)),
    quitAction(bind(&App::SetQuitFlag, this)),
    mainWindow(hInstance)
{
    if (instance != nullptr) {
        throw Exception("App has already been instantiated.");
    }
    instance = this;

    mainWindow.Init();

    // Create the platform window and subscribe to relevant events.
    mainWindow.OnClose += &quitAction;
    
    mainWindow.OnDeactivate += &pauseAction;
    mainWindow.OnEnterSizeMove += &pauseAction;
    mainWindow.OnMinimize += &pauseAction; // TODO: this should be an option in settings.
    
    mainWindow.OnActivate += &unpauseAction;

    mainWindow.OnExitSizeMove += &unpauseAction;
    mainWindow.OnExitSizeMove += &resizeWindowAction;
    mainWindow.OnMaximize += &unpauseAction;
    mainWindow.OnMaximize += &resizeWindowAction;
    mainWindow.OnRestored += &unpauseAction;
    mainWindow.OnRestored += &resizeWindowAction;

    auto windowSize = mainWindow.GetWindowSize();
    Gfx::Instance().Init(mainWindow.GetWindowHandler(), windowSize.x, windowSize.y);
}


App::~App()
{
    mainWindow.OnClose -= &quitAction;

    mainWindow.OnDeactivate -= &pauseAction;
    mainWindow.OnEnterSizeMove -= &pauseAction;
    mainWindow.OnMinimize -= &pauseAction; // TODO: this should be an option in settings.

    mainWindow.OnActivate -= &unpauseAction;

    mainWindow.OnExitSizeMove -= &unpauseAction;
    mainWindow.OnExitSizeMove -= &resizeWindowAction;
    mainWindow.OnMaximize -= &unpauseAction;
    mainWindow.OnMaximize -= &resizeWindowAction;
    mainWindow.OnRestored -= &unpauseAction;
    mainWindow.OnRestored -= &resizeWindowAction;
}


/// Main Loop.
void App::Run()
{
    timer.Reset();

    while (!quitting) {
        if (!mainWindow.Update()) {
            timer.Update();
        }
    }
}


void App::ResizeWindow()
{
    // TODO: graphics update.
}


void App::Unpause() {
    timer.Start();
    paused = false;
}


void App::Pause() {
    timer.Stop();
    paused = true;
}


void App::SetQuitFlag() {
    quitting = true;
}
