/*#include <iostream>
#include <Windows.h>

HHOOK hook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
) {
    std::cout << "Hooked and callback hit \n";
    std::cout << nCode;
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            std::cout << "Key Pressed: " << pKeyStruct->vkCode << std::endl;
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}


int main()
{
    std::cout << "Starting test\n";

    //13 is WH_KEYBOARD_LL
    
    hook = SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    std::cout << "Hooked \n";

    if (hook == NULL) { std::cout << GetLastError(); }

    while (true) {}

}*/


#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <ctime>


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

int main() {

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cerr << "Failed to set hook" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    while(true) {}

    UnhookWindowsHookEx(hHook);

    return 0;
}
