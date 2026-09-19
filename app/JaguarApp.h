#pragma once

#include "ui/MainWindow/JaguarStudioWindow.h"
#include <memory>

namespace jaguar {

class JaguarApp {
public:
    JaguarApp();
    ~JaguarApp();

    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    int Run();

private:
    std::unique_ptr<JaguarStudioWindow> m_window;
};

} // namespace jaguar
