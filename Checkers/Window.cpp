#include "Window.h"
#include "Types.h"

#include <unordered_map>


std::unordered_map<HWND, Window*> windows;

LRESULT CALLBACK Window::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_KEYDOWN:
            if (wParam == 'Q') {
                InvalidateWindow();
            }
            else if (wParam == 'R') {
                m_board.Reset();
            }

            break;
        case WM_LBUTTONDOWN:
            POINTS point = MAKEPOINTS(lParam);
            TranslateClick(point);
            break;
        default:
            return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

Window::Window(HINSTANCE hInst)
{
    m_windowClass.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
        std::unordered_map<HWND, Window*>::iterator it = windows.find(hWnd);
        return it != windows.end() ? it->second->WndProc(uMsg, wParam, lParam) : DefWindowProc(hWnd, uMsg, wParam, lParam);
    };

    m_windowClass.hInstance = hInst;
    m_windowClass.lpszClassName = L"Checkers Window";
    m_windowClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    m_windowClass.hCursor = LoadCursorFromFile(L"arrow.cur");
    m_hWnd = (HWND) NULL;
    m_windowValid = true;
}

bool Window::Begin()
{
    if (!RegisterClass(&m_windowClass)) {
        return false;
    }

    m_hWnd = CreateWindow(L"Checkers Window", L"Checkers", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 700, (HWND) NULL, (HMENU) NULL, m_windowClass.hInstance, (LPVOID) NULL);
    if (!m_hWnd) {
        return false;
    }

    windows.insert(std::make_pair(m_hWnd, this)); 
    
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);

    m_board.InitializeBoard(BOARD_LENGTH, GetDC(m_hWnd));
    m_board.SetupBoard();
    m_board.DrawBoard();

    MessageLoop();

    return true;
}

void Window::MessageLoop()
{
    MSG msg;

    while (m_windowValid) {
        bool message = PeekMessageA(&msg, m_hWnd, 0, 0, PM_REMOVE);
        if (message) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Window::InvalidateWindow()
{
    m_windowValid = false;
}

void Window::TranslateClick(POINTS point)
{
    int x = point.x;
    int y = point.y;
    int row = -1;
    int column = -1;

    while (x > 49) {
        ++column;
        x -= 50;
    }
    while (y > 49) {
        ++row;
        y -= 50;
    }
    m_board.Select(row, column);
}
