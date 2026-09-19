#pragma once

#include "ui/MainWindow/UITheme.h"
#include "ui/MainWindow/Navigation.h"
#include "ui/MainWindow/StatusSystem.h"
#include "core/Project/ProjectModel.h"
#include "editor/Preview/PreviewEngine.h"
#include <windows.h>
#include <string>
#include <memory>

namespace jaguar {

class JaguarStudioWindow {
public:
    JaguarStudioWindow();
    ~JaguarStudioWindow();

    bool Create(HINSTANCE hInstance, int nCmdShow);
    int RunMessageLoop();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void Paint(HDC hdc, const RECT& clientRect);
    void PaintSidebar(HDC hdc, const RECT& rect);
    void PaintHeader(HDC hdc, const RECT& rect);
    void PaintFooter(HDC hdc, const RECT& rect);
    void PaintMainView(HDC hdc, const RECT& rect);

    void PaintHomeView(HDC hdc, const RECT& rect);
    void PaintRecordView(HDC hdc, const RECT& rect);
    void PaintEditorView(HDC hdc, const RECT& rect);
    void PaintProjectsView(HDC hdc, const RECT& rect);
    void PaintMediaView(HDC hdc, const RECT& rect);
    void PaintExportView(HDC hdc, const RECT& rect);
    void PaintSettingsView(HDC hdc, const RECT& rect);

    void HandleLButtonDown(int x, int y);

    HINSTANCE m_hInstance{ nullptr };
    HWND m_hwnd{ nullptr };
    UITheme m_theme;
    ViewType m_currentView{ ViewType::Home };
    PreviewEngine m_previewEngine;

    // Recording controls state
    bool m_isRecording{ false };
    bool m_isRecordingPaused{ false };
    int m_recordingSeconds{ 0 };
};

} // namespace jaguar
