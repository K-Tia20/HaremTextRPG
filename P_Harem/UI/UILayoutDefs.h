#pragma once

/**
 * @namespace UI_LAYOUT
 * @brief UI 요소들의 좌표 및 크기 상수를 정의합니다.
 */
namespace UI_LAYOUT {
    // [물리적 설계] 실제 콘솔 창 크기
    constexpr int CONSOLE_WIDTH = 160;
    constexpr int CONSOLE_HEIGHT = 45;

    // [근본적 해결] 버퍼와 창 크기를 1:1로 일치시킵니다.
    constexpr int BUFFER_WIDTH = CONSOLE_WIDTH;
    constexpr int BUFFER_HEIGHT = CONSOLE_HEIGHT;

    // [중요] 44번 행은 'Dead Zone'으로 설정하여 절대 쓰지 않습니다. (스크롤 방지 핵심)
    constexpr int MAX_SAFE_Y = CONSOLE_HEIGHT - 2; // 43

    // LEFT PANEL (SMARTPHONE)
    constexpr int LEFT_PANEL_X = 0;
    constexpr int LEFT_PANEL_Y = 0;
    constexpr int LEFT_PANEL_W = 40;
    constexpr int LEFT_PANEL_H = CONSOLE_HEIGHT - 1; 

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
    // 하단 입력창 여백을 위해 높이 조절
    constexpr int PHONE_H = 30; 

    constexpr int INPUT_BOX_X = LEFT_PANEL_X;
    constexpr int INPUT_BOX_Y = PHONE_Y + PHONE_H; 
    constexpr int INPUT_BOX_W = LEFT_PANEL_W;
    constexpr int INPUT_BOX_H = 4; // 40~43 (MAX_SAFE_Y 43에 맞춰 종료)

    // RIGHT PANEL (MAIN CONTENT)
    constexpr int RIGHT_AREA_X = LEFT_PANEL_W;
    
    constexpr int VIEWPORT_X = RIGHT_AREA_X;
    constexpr int VIEWPORT_Y = 0;
    constexpr int VIEWPORT_W = CONSOLE_WIDTH - LEFT_PANEL_W;
    constexpr int VIEWPORT_H = 30;

    constexpr int LOG_X = RIGHT_AREA_X;
    constexpr int LOG_Y = VIEWPORT_H;
    constexpr int LOG_W = VIEWPORT_W;
    // 로그창 역시 43번 줄에서 끝나도록 설정
    constexpr int LOG_H = MAX_SAFE_Y - LOG_Y + 1; 
}

#define UI_BRUSH "█"
