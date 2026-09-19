#pragma once

#include <windows.h>

namespace jaguar {

struct UITheme {
    COLORREF backgroundColor{ RGB(24, 24, 28) };
    COLORREF sidebarColor{ RGB(18, 18, 22) };
    COLORREF cardColor{ RGB(32, 32, 38) };
    COLORREF headerColor{ RGB(28, 28, 34) };
    COLORREF accentColor{ RGB(255, 128, 0) }; // Jaguar Orange
    COLORREF textColor{ RGB(240, 240, 245) };
    COLORREF subtextColor{ RGB(160, 160, 170) };
    COLORREF buttonColor{ RGB(45, 45, 55) };
    COLORREF buttonHoverColor{ RGB(60, 60, 72) };
    COLORREF timelineTrackColor{ RGB(38, 38, 46) };
    COLORREF timelineClipColor{ RGB(0, 122, 204) };
    COLORREF timelinePlayheadColor{ RGB(255, 60, 60) };
};

} // namespace jaguar
