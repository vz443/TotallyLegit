#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <ctime>

std::vector<char> buf;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            auto kbstruct = (KBDLLHOOKSTRUCT*)lParam;
            char key = MapVirtualKey(kbstruct->vkCode, MAPVK_VK_TO_CHAR);
            std::cout << "Hit with key" << key << "\n";
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
    LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (event == EVENT_SYSTEM_FOREGROUND && hwnd != NULL) {
        char windowTitle[256];
        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
        std::cout << "Active Window Changed: " << windowTitle << "\n";
    }
}


int main() {

    HWINEVENTHOOK windowHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        nullptr, WinEventProc, 0, 0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );


    if (!windowHook) {
        std::cerr << "Failed to install hook." << std::endl;
        return 1;
    }

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cerr << "Failed to set hook" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_QUIT) {
            //std::cout << "QUITTED";
            MessageBoxA(NULL, "Test", "Test Title", 0);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWinEvent(windowHook);
    UnhookWindowsHookEx(hHook);

    return 0;
}
