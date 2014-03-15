

#include "baseapp.h"


BaseApp::BaseApp(HINSTANCE hInstance, LPSTR cmdLine)
: hWnd(nullptr), hInstance(hInstance), hCursor(0), windowTitle("gles3mark"), className("gles3markWindow"),
  quit(false), exitCode(0) {

    // parse params       
}

BaseApp::~BaseApp() {
    if (hWnd) {
        DestroyWindow();
    }
}

bool BaseApp::SetupWindow(int w, int h) {
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = className.c_str();
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = SystemWindowProc;
    wc.cbWndExtra = sizeof(BaseApp*);
    RegisterClass(&wc);

    width = w;
    height = h;

    RECT winSize;
    winSize.left = 0;
    winSize.top = 0;
    winSize.right = width;
    winSize.bottom = height;
    AdjustWindowRect(&winSize, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindow(className.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winSize.right - winSize.left,
        winSize.bottom - winSize.top,
        nullptr, nullptr, hInstance, (LPVOID)this);

    hCursor = LoadCursor(nullptr, IDC_CROSS);

    POINT center = { width / 2, height / 2 };
    ::ClientToScreen(hWnd, &center);
    windowCenter = center;

    ::SetFocus(hWnd);
    ::ShowWindow(hWnd, true);

    return (hWnd != nullptr);
}

void BaseApp::DestroyWindow() {
    if (hWnd == nullptr)
        return;
    
    ::DestroyWindow(hWnd);
    UnregisterClass(className.c_str(), hInstance);
    hWnd = nullptr;
    hCursor = 0;
    width = 0;
    height = 0;
}

void BaseApp::ShowWindow(bool visible) {
    ::ShowWindow(hWnd, visible ? SW_SHOW : SW_HIDE);
}

void BaseApp::SetWindowTitle(const std::string& title) {
    windowTitle = title;
    if (hWnd)
        ::SetWindowText(hWnd, windowTitle.c_str());
}

void BaseApp::ShowMsg(const std::string& msg, UINT type) {
    std::string caption;

    switch (type) {
        case MB_ICONINFORMATION:
            caption = "Info";
            break;
        case MB_ICONERROR:
            caption = "Error";
            break;
        case MB_ICONWARNING:
            caption = "Warning";
            break;
    }
    
    MessageBox(hWnd, msg.c_str(), caption.c_str(), type);
}

LRESULT CALLBACK BaseApp::SystemWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    BaseApp* self;

    if (msg == WM_NCCREATE) {
        self = static_cast<BaseApp*>(((LPCREATESTRUCT)lp)->lpCreateParams);

        if (self) {
            SetWindowLongPtr(hwnd, 0, (LONG_PTR)self);
            self->hWnd = hwnd;
        }
    }
    else {
        self = (BaseApp*)GetWindowLongPtr(hwnd, 0); // UPInt
    }

    return self ? self->WindowProc(msg, wp, lp) :
        DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT BaseApp::WindowProc(UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {
        case WM_PAINT: {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
            return 0;

        case WM_SETCURSOR:
            ::SetCursor(hCursor);
            return 0;

        case WM_MOUSEMOVE:


            break;

        case WM_MOVE: {
                RECT r;
                GetClientRect(hWnd, &r);
                windowCenter.x = r.right / 2;
                windowCenter.y = r.bottom / 2;
                ::ClientToScreen(hWnd, &windowCenter);
            }
            break;

        case WM_SETFOCUS :

            break;

        case WM_SIZE:
            if (wp != SIZE_MINIMIZED) {
                OnResize(LOWORD(lp), HIWORD(lp));
            }
            break;

        case WM_STYLECHANGING: {
                RECT winSize;
                winSize.left = 0;
                winSize.top = 0;
                winSize.right = width;
                winSize.bottom = height;
                
                int w = winSize.right - winSize.left;
                int h = winSize.bottom - winSize.top;
                
                STYLESTRUCT* pss = (STYLESTRUCT*)lp;
                AdjustWindowRect(&winSize, pss->styleNew, false);
                ::SetWindowPos(hWnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
            }
            break;

        case WM_QUIT:
        case WM_CLOSE:
            OnQuit();
            return false;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}

void BaseApp::Exit(int exitCode) {
    quit = true;
    this->exitCode = exitCode;
}

int BaseApp::Run() {

    OnStartup();

    if (hWnd == nullptr) {
        return exitCode;
    }

    while (!quit) {
        MSG msg;
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            OnIdle();
            
            if (IsIconic(hWnd)) {
                Sleep(10);
            }
        }
    }

    return exitCode;
}