#include "JaguarApp.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)pCmdLine;
    jaguar::JaguarApp app;
    if (!app.Initialize(hInstance, nCmdShow)) {
        return 0;
    }
    return app.Run();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)pCmdLine;
    jaguar::JaguarApp app;
    if (!app.Initialize(hInstance, nCmdShow)) {
        return 0;
    }
    return app.Run();
}
#endif

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    jaguar::JaguarApp app;
    if (!app.Initialize(hInstance, SW_SHOW)) {
        return 0;
    }
    return app.Run();
}
