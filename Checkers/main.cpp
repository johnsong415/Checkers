#include "framework.h"
#include "Checkers.h"
#include "Window.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Window window(hInstance);

    bool result = window.Begin();
    if (!result) {
        // TODO: log...
        return -1;
    }

    return 0;
}
