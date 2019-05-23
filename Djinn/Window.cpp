#include "Window.h"

#include <windowsx.h>

using namespace Djn;

// Globals
LRESULT WINAPI MainWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return MainWindow::GetInstance()->WindowProcedure(hWnd, msg, wParam, lParam);
}

// Statics
MainWindow* MainWindow::GetInstance() {
    return MainWindow::Instance;
}

MainWindow* MainWindow::Instance = nullptr;

// Instance.
MainWindow::MainWindow(HINSTANCE hInstance) : _hInstance(hInstance)
{
    if (MainWindow::Instance != nullptr) {
        throw Exception("static MainWindow instance is not null. Cannot create multiple MainWindows.");
    }

    MainWindow::Instance = this;
}

MainWindow::~MainWindow() {}

HWND MainWindow::GetWindowHandler() const
{
    return _hWnd;
}

int2 MainWindow::GetWindowOrigin() const
{
    return _windowOrigin;
}

int2 MainWindow::GetWindowSize() const
{
    return _windowSize;
}

void MainWindow::Init() {
    std::wstring longName = L"EnDjinn";
    std::wstring shortName = L"EnDjinn";
    std::wstring className = shortName + L"WindowClass";

    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    wc.lpszMenuName = 0;
    wc.lpszClassName = className.c_str();

    if (!RegisterClass(&wc)) {
        throw Exception("Failed to register window class.");
    }

    _windowOrigin = { 0, 0 };
    _windowSize = { 512, 512 };
    
    _hWnd = CreateWindow(
        className.c_str(),
        longName.c_str(),
        WS_OVERLAPPEDWINDOW,
        _windowOrigin.x, _windowOrigin.y,
        _windowSize.x, _windowSize.y,
        NULL,
        NULL,
        _hInstance,
        NULL
    );

    ShowWindow(_hWnd, SW_SHOW);
    UpdateWindow(_hWnd);

    if (_hWnd == NULL) {
        throw Exception("Unable to create hWnd");
    }
}

bool MainWindow::Update() {
    MSG msg = {};
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
    return false;
}

LRESULT MainWindow::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /* Handling the following messages:
    * ACTIVATE
    * SIZE
    * ENTERSIZEMOVE
    * EXITSIZEMOVE
    * DESTROY
    * MENUCHAR
    * GETMINMAXINFO
    * L- M- RBUTTONDOWN
    * L- M- RBUTTONUP
    * MOUSEMOVE
    * KEYUP
    */

    switch (msg) {
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE) {
            OnDeactivate();
        } else {
            OnActivate();
        }
        return 0;
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) {
            windowState = Minimized;
            OnMinimize();
        } else if (wParam == SIZE_MAXIMIZED) {
            windowState = Maximized;
            OnMaximize();
        } else if (wParam == SIZE_RESTORED) {
            switch (windowState) {
            case Minimized:
                windowState = Windowed;
                OnRestored();
                break;
            case Maximized:
                windowState = Windowed;
                OnRestored();
                break;
            default:
                OnRestored();
                break;
            }
        }
        return 0;
    case WM_ENTERSIZEMOVE:
        windowState = Resizing;
        OnEnterSizeMove();
        return 0;
    case WM_EXITSIZEMOVE:
        windowState = Windowed;
        OnExitSizeMove();
        return 0;
    case WM_DESTROY:
        OnClose();
        PostQuitMessage(0);
        return 0;
    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
    case WM_GETMINMAXINFO:
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 200;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 200;
        return 0;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_MOUSEMOVE:
        OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_KEYUP:
        if (wParam == VK_ESCAPE) {
            OnClose();
            PostQuitMessage(0);
        }
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void MainWindow::OnMouseDown(WPARAM wParam, int x, int y) {}
void MainWindow::OnMouseUp(WPARAM wParam, int x, int y) {}
void MainWindow::OnMouseMove(WPARAM wParam, int x, int y) {}

