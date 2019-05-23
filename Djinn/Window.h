#pragma once

#include <Windows.h>

#include "Djinn.h"
#include "Timer.h"

#include "Event.h"

namespace Djn
{
    typedef enum {
        Minimized,
        Windowed,
        Maximized,
        Resizing
    } WindowState;

    class MainWindow {
    public:
        static MainWindow* GetInstance();
    private:
        static MainWindow* Instance;
    public:
        MainWindow(HINSTANCE hInstance);
        MainWindow(const MainWindow& other) = delete;
        ~MainWindow();
        HWND GetWindowHandler() const;
        int2 GetWindowOrigin() const;
        int2 GetWindowSize() const;
        void Init();
        bool Update();

        LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        Event<> OnActivate;
        Event<> OnDeactivate;
        Event<> OnMinimize;
        Event<> OnMaximize;
        Event<> OnRestored;
        Event<> OnEnterSizeMove;
        Event<> OnExitSizeMove;
        Event<> OnClose;
    private:
        HINSTANCE _hInstance;
        HWND _hWnd;
        int2 _windowOrigin;
        int2 _windowSize;
        WindowState windowState = Windowed;
        void OnMouseDown(WPARAM wParam, int x, int y);
        void OnMouseUp(WPARAM wParam, int x, int y);
        void OnMouseMove(WPARAM wParam, int x, int y);
    };
}
