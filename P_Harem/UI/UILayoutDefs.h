#pragma once

/**
 * @namespace UI_LAYOUT
 * @brief UI 요소들의 좌표 및 크기 상수를 정의합니다.
 */
namespace UI_LAYOUT {
    constexpr int CONSOLE_WIDTH = 160;
    constexpr int CONSOLE_HEIGHT = 45;

    // LEFT PANEL (SMARTPHONE)
    constexpr int LEFT_PANEL_X = 0;
    constexpr int LEFT_PANEL_Y = 0;
    constexpr int LEFT_PANEL_W = 40;
    constexpr int LEFT_PANEL_H = CONSOLE_HEIGHT;

    constexpr int DATE_X = LEFT_PANEL_X;
    constexpr int DATE_Y = 0;
    constexpr int DATE_W = LEFT_PANEL_W;
    constexpr int DATE_H = 3;

    constexpr int USER_INFO_X = LEFT_PANEL_X;
    constexpr int USER_INFO_Y = DATE_H;
    constexpr int USER_INFO_W = LEFT_PANEL_W;
    constexpr int USER_INFO_H = 7;

    constexpr int PHONE_X = LEFT_PANEL_X;
    constexpr int PHONE_Y = DATE_H + USER_INFO_H;
    constexpr int PHONE_W = LEFT_PANEL_W;
    constexpr int PHONE_H = LEFT_PANEL_H - PHONE_Y;

    // RIGHT PANEL (MAIN CONTENT)
    constexpr int RIGHT_AREA_X = LEFT_PANEL_W;
    
    constexpr int VIEWPORT_X = RIGHT_AREA_X;
    constexpr int VIEWPORT_Y = 0;
    constexpr int VIEWPORT_W = CONSOLE_WIDTH - LEFT_PANEL_W;
    constexpr int VIEWPORT_H = 30;

    constexpr int LOG_X = RIGHT_AREA_X;
    constexpr int LOG_Y = VIEWPORT_H;
    constexpr int LOG_W = VIEWPORT_W;
    constexpr int LOG_H = CONSOLE_HEIGHT - VIEWPORT_H;
}

#define UI_BRUSH "█"
