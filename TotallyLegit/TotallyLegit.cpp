#include <iostream>
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
    
    hook = SetWindowsHookExW(13, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    std::cout << "Hooked \n";

    if (hook == NULL) { std::cout << GetLastError(); }

    while (true) {}

}

