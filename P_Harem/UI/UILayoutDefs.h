#pragma once

namespace UI_LAYOUT {
    // 전체 콘솔 설정
    constexpr int CONSOLE_WIDTH = 160;
    constexpr int CONSOLE_HEIGHT = 45;

    // 좌측 패널 (스마트폰 컨셉)
    constexpr int LEFT_PANEL_X = 0;
    constexpr int LEFT_PANEL_Y = 0;
    constexpr int LEFT_PANEL_W = 40;
    constexpr int LEFT_PANEL_H = CONSOLE_HEIGHT;

    // 날짜 창 (독립)
    constexpr int DATE_X = LEFT_PANEL_X;
    constexpr int DATE_Y = 0;
    constexpr int DATE_W = LEFT_PANEL_W;
    constexpr int DATE_H = 3;

    // 유저 정보
    constexpr int USER_INFO_X = LEFT_PANEL_X;
    constexpr int USER_INFO_Y = DATE_H; // 3
    constexpr int USER_INFO_W = LEFT_PANEL_W;
    constexpr int USER_INFO_H = 7;

    // 히로인 목록 (스마트폰 화면 내부)
    constexpr int PHONE_X = LEFT_PANEL_X;
    constexpr int PHONE_Y = DATE_H + USER_INFO_H; // 10
    constexpr int PHONE_W = LEFT_PANEL_W;
    constexpr int PHONE_H = LEFT_PANEL_H - PHONE_Y;

    // 우측 메인 영역
    constexpr int RIGHT_AREA_X = LEFT_PANEL_W;
    
    // 뷰포트 (이미지 출력)
    constexpr int VIEWPORT_X = RIGHT_AREA_X;
    constexpr int VIEWPORT_Y = 0;
    constexpr int VIEWPORT_W = CONSOLE_WIDTH - LEFT_PANEL_W; // 120
    constexpr int VIEWPORT_H = 30;

    // 메시지 로그
    constexpr int LOG_X = RIGHT_AREA_X;
    constexpr int LOG_Y = VIEWPORT_H;
    constexpr int LOG_W = VIEWPORT_W;
    constexpr int LOG_H = CONSOLE_HEIGHT - VIEWPORT_H; // 15
}

// UI 테두리에 사용할 문자 (1칸 너비 보장되는 블록 권장)
#define UI_BRUSH "█"
