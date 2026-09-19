#include "JaguarStudioWindow.h"
#include "core/Logging/Logger.h"
#include "core/Settings/SettingsManager.h"
#include "editor/Timeline/TimelineEngine.h"
#include "export/ExportQueue/ExportQueue.h"
#include <iostream>

namespace jaguar {

JaguarStudioWindow::JaguarStudioWindow() = default;
JaguarStudioWindow::~JaguarStudioWindow() = default;

bool JaguarStudioWindow::Create(HINSTANCE hInstance, int nCmdShow) {
    m_hInstance = hInstance;

    const wchar_t CLASS_NAME[] = L"JaguarStudioNativeClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = JaguarStudioWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(m_theme.backgroundColor);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    m_hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Jaguar Studio - Native Windows Video Recorder & Editor v1.0.0",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 800,
        NULL,
        NULL,
        hInstance,
        this
    );

    if (!m_hwnd) return false;

    ShowWindow(m_hwnd, nCmdShow);
    UpdateWindow(m_hwnd);

    LOG_INFO("UI", "Jaguar Studio main window created successfully.");
    return true;
}

int JaguarStudioWindow::RunMessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK JaguarStudioWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    JaguarStudioWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<JaguarStudioWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<JaguarStudioWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT JaguarStudioWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
        HandleLButtonDown(LOWORD(lParam), HIWORD(lParam));
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        Paint(hdc, clientRect);
        EndPaint(hwnd, &ps);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void JaguarStudioWindow::HandleLButtonDown(int x, int y) {
    if (x < 180) { // Sidebar clicked
        int itemHeight = 45;
        int index = (y - 60) / itemHeight;
        if (index >= 0 && index <= 6) {
            m_currentView = static_cast<ViewType>(index);
        }
    }
}

void JaguarStudioWindow::Paint(HDC hdc, const RECT& clientRect) {
    RECT sidebarRect = clientRect;
    sidebarRect.right = 180;

    RECT headerRect = clientRect;
    headerRect.left = 180;
    headerRect.bottom = 50;

    RECT footerRect = clientRect;
    footerRect.left = 180;
    footerRect.top = clientRect.bottom - 30;

    RECT mainRect = clientRect;
    mainRect.left = 180;
    mainRect.top = 50;
    mainRect.bottom = clientRect.bottom - 30;

    PaintSidebar(hdc, sidebarRect);
    PaintHeader(hdc, headerRect);
    PaintFooter(hdc, footerRect);
    PaintMainView(hdc, mainRect);
}

void JaguarStudioWindow::PaintSidebar(HDC hdc, const RECT& rect) {
    HBRUSH hBrush = CreateSolidBrush(m_theme.sidebarColor);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.accentColor);
    RECT brandRect = rect;
    brandRect.bottom = 60;
    DrawTextW(hdc, L" JAGUAR STUDIO", -1, &brandRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    const wchar_t* navItems[] = { L"  Home", L"  Record", L"  Editor", L"  Projects", L"  Media", L"  Export", L"  Settings" };
    int yOffset = 60;
    for (int i = 0; i < 7; ++i) {
        RECT itemRect = rect;
        itemRect.top = yOffset;
        itemRect.bottom = yOffset + 45;

        if (static_cast<ViewType>(i) == m_currentView) {
            HBRUSH activeBrush = CreateSolidBrush(m_theme.cardColor);
            FillRect(hdc, &itemRect, activeBrush);
            DeleteObject(activeBrush);
            SetTextColor(hdc, m_theme.accentColor);
        } else {
            SetTextColor(hdc, m_theme.textColor);
        }

        DrawTextW(hdc, navItems[i], -1, &itemRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        yOffset += 45;
    }
}

void JaguarStudioWindow::PaintHeader(HDC hdc, const RECT& rect) {
    HBRUSH hBrush = CreateSolidBrush(m_theme.headerColor);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);
    std::wstring title = L"  " + std::wstring(ProjectManager::Instance().GetCurrentProject().name.begin(), ProjectManager::Instance().GetCurrentProject().name.end());
    DrawTextW(hdc, title.c_str(), -1, (LPRECT)&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void JaguarStudioWindow::PaintFooter(HDC hdc, const RECT& rect) {
    HBRUSH hBrush = CreateSolidBrush(m_theme.headerColor);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.subtextColor);
    std::string status = " Status: " + StatusSystem::Instance().GetLastNotification();
    std::wstring wStatus(status.begin(), status.end());
    DrawTextW(hdc, wStatus.c_str(), -1, (LPRECT)&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void JaguarStudioWindow::PaintMainView(HDC hdc, const RECT& rect) {
    HBRUSH hBrush = CreateSolidBrush(m_theme.backgroundColor);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    switch (m_currentView) {
    case ViewType::Home: PaintHomeView(hdc, rect); break;
    case ViewType::Record: PaintRecordView(hdc, rect); break;
    case ViewType::Editor: PaintEditorView(hdc, rect); break;
    case ViewType::Projects: PaintProjectsView(hdc, rect); break;
    case ViewType::Media: PaintMediaView(hdc, rect); break;
    case ViewType::Export: PaintExportView(hdc, rect); break;
    case ViewType::Settings: PaintSettingsView(hdc, rect); break;
    }
}

void JaguarStudioWindow::PaintHomeView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT titleRect = rect;
    titleRect.left += 30;
    titleRect.top += 30;
    DrawTextW(hdc, L"Welcome to Jaguar Studio", -1, &titleRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT card1 = rect;
    card1.left += 30; card1.top += 80; card1.right = card1.left + 220; card1.bottom = card1.top + 120;
    HBRUSH brush1 = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card1, brush1);
    DeleteObject(brush1);
    DrawTextW(hdc, L"\n  + New Recording\n  Start screen capture", -1, &card1, DT_LEFT | DT_TOP);

    RECT card2 = rect;
    card2.left += 270; card2.top += 80; card2.right = card2.left + 220; card2.bottom = card2.top + 120;
    HBRUSH brush2 = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card2, brush2);
    DeleteObject(brush2);
    DrawTextW(hdc, L"\n  + New Project\n  Create video project", -1, &card2, DT_LEFT | DT_TOP);
}

void JaguarStudioWindow::PaintRecordView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT tRect = rect; tRect.left += 30; tRect.top += 30;
    DrawTextW(hdc, L"Recording Workspace", -1, &tRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT card = rect;
    card.left += 30; card.top += 80; card.right = rect.right - 30; card.bottom = card.top + 300;
    HBRUSH brush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card, brush);
    DeleteObject(brush);

    RECT textR = card; textR.left += 20; textR.top += 20;
    DrawTextW(hdc, L"Source: Full Screen (Primary Monitor 1920x1080)\nWebcam: Integrated HD Webcam (1280x720 30FPS)\nAudio: Default Microphone & System Audio Loopback\n\nStatus: Ready to Record", -1, &textR, DT_LEFT | DT_TOP);
}

void JaguarStudioWindow::PaintEditorView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    RECT previewRect = rect;
    previewRect.right = rect.left + (width * 2 / 3);
    previewRect.bottom = rect.top + (height / 2);
    HBRUSH previewBrush = CreateSolidBrush(RGB(15, 15, 18));
    FillRect(hdc, &previewRect, previewBrush);
    DeleteObject(previewBrush);
    DrawTextW(hdc, L"Video Preview (Direct3D Frame Compositor)", -1, &previewRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT binRect = rect;
    binRect.left = previewRect.right + 10;
    binRect.bottom = previewRect.bottom;
    HBRUSH binBrush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &binRect, binBrush);
    DeleteObject(binBrush);
    DrawTextW(hdc, L" Media Bin / Inspector", -1, &binRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT timelineRect = rect;
    timelineRect.top = previewRect.bottom + 10;
    HBRUSH tlBrush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &timelineRect, tlBrush);
    DeleteObject(tlBrush);

    RECT trackR = timelineRect; trackR.left += 10; trackR.top += 10;
    DrawTextW(hdc, L"Timeline Tracks:\n [V1] Video Track 1  |  Clip_1.mp4 (00:00 - 00:10)\n [V2] Overlay Track  |  Webcam_Overlay.png\n [A1] Audio Track 1  |  Mic_Audio.wav\n [A2] System Audio   |  Desktop_Audio.wav", -1, &trackR, DT_LEFT | DT_TOP);
}

void JaguarStudioWindow::PaintProjectsView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT tRect = rect; tRect.left += 30; tRect.top += 30;
    DrawTextW(hdc, L"Recent Projects (.jaguar)", -1, &tRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT card = rect;
    card.left += 30; card.top += 80; card.right = card.left + 250; card.bottom = card.top + 150;
    HBRUSH brush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card, brush);
    DeleteObject(brush);

    RECT textR = card; textR.left += 15; textR.top += 15;
    DrawTextW(hdc, L"Untitled Project.jaguar\nModified: Just now\nTracks: 6 | Duration: 00:10", -1, &textR, DT_LEFT | DT_TOP);
}

void JaguarStudioWindow::PaintMediaView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT tRect = rect; tRect.left += 30; tRect.top += 30;
    DrawTextW(hdc, L"Media Library & Waveform Cache", -1, &tRect, DT_LEFT | DT_TOP | DT_SINGLELINE);
}

void JaguarStudioWindow::PaintExportView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT tRect = rect; tRect.left += 30; tRect.top += 30;
    DrawTextW(hdc, L"Export Engine & Queue", -1, &tRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT card = rect;
    card.left += 30; card.top += 80; card.right = rect.right - 30; card.bottom = card.top + 200;
    HBRUSH brush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card, brush);
    DeleteObject(brush);

    RECT textR = card; textR.left += 20; textR.top += 20;
    DrawTextW(hdc, L"Preset: 1080p Web (MP4 - H264 / AAC 60FPS)\nOutput File: C:\\JaguarRecordings\\ExportedVideo.mp4\nEstimated Size: ~12.5 MB\n\nStatus: Ready to Export", -1, &textR, DT_LEFT | DT_TOP);
}

void JaguarStudioWindow::PaintSettingsView(HDC hdc, const RECT& rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_theme.textColor);

    RECT tRect = rect; tRect.left += 30; tRect.top += 30;
    DrawTextW(hdc, L"Jaguar Studio Settings", -1, &tRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    RECT card = rect;
    card.left += 30; card.top += 80; card.right = rect.right - 30; card.bottom = card.top + 300;
    HBRUSH brush = CreateSolidBrush(m_theme.cardColor);
    FillRect(hdc, &card, brush);
    DeleteObject(brush);

    RECT textR = card; textR.left += 20; textR.top += 20;
    DrawTextW(hdc, L"Theme: Dark Professional\nDefault FPS: 60 FPS\nGPU Acceleration: Enabled (Direct3D / WASAPI / Media Foundation)\nCache Folder: C:\\JaguarCache\nWorker Threads: 4", -1, &textR, DT_LEFT | DT_TOP);
}

} // namespace jaguar
