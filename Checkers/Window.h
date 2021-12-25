#pragma once

#include "framework.h"
#include "Board.h"


class Window
{
public:
    Window(HINSTANCE hInst);
    bool Begin();
    void MessageLoop();
    void InvalidateWindow();
    void TranslateClick(POINTS point);

public:
    LRESULT CALLBACK WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    WNDCLASS m_windowClass;
    HWND m_hWnd;
    bool m_windowValid;
    Board m_board;
};
