#pragma once // 이 파일이 여러 번 불려와서 컴퓨터가 헷갈려 하는 것을 방지하는 안전장치란다.

/**
 * [10년 뒤의 너에게]
 * 이 파일은 게임 화면의 '지도'이자 '설계도'란다. 
 * 화면 어디에 무엇을 그릴지 숫자로 미리 약속해두는 곳이지. 
 * 여기서 숫자를 한 번 정해두면, 나중에 화면 크기를 바꾸고 싶을 때 이 파일만 고치면 모든 게 자동으로 맞춰진단다.
 */
namespace UI_LAYOUT { 
    // 전체 도화지의 크기를 정해요.
    constexpr int CONSOLE_WIDTH = 160;  // 가로 너비는 160칸!
    constexpr int CONSOLE_HEIGHT = 45;  // 세로 높이는 45줄로 정했어.

    // [좌측 패널: 스마트폰 영역]
    constexpr int LEFT_PANEL_X = 0;     // 왼쪽 패널은 화면 맨 왼쪽(0)에서 시작해.
    constexpr int LEFT_PANEL_Y = 0;     // 맨 위쪽(0)부터 시작하지.
    constexpr int LEFT_PANEL_W = 40;    // 가로로 40칸의 공간을 차지한단다.
    constexpr int LEFT_PANEL_H = CONSOLE_HEIGHT; // 높이는 화면 끝까지 다 써요.

    // 날짜 창 (스마트폰 맨 위)
    constexpr int DATE_X = LEFT_PANEL_X; 
    constexpr int DATE_Y = 0;            
    constexpr int DATE_W = LEFT_PANEL_W; 
    constexpr int DATE_H = 3;            // 딱 3줄만 쓰기로 약속했어.

    // 유저 정보 창 (날짜 창 바로 아래)
    constexpr int USER_INFO_X = LEFT_PANEL_X;
    constexpr int USER_INFO_Y = DATE_H;  // 날짜 창(3줄)이 끝나는 곳에서 시작해.
    constexpr int USER_INFO_W = LEFT_PANEL_W;
    constexpr int USER_INFO_H = 7;       // 이름과 돈을 보여줄 7줄의 공간이야.

    // 히로인 목록 창 (스마트폰의 실제 화면 부분)
    constexpr int PHONE_X = LEFT_PANEL_X;
    constexpr int PHONE_Y = DATE_H + USER_INFO_H; // 날짜(3)+정보(7) = 10번 줄부터 시작!
    constexpr int PHONE_W = LEFT_PANEL_W;
    constexpr int PHONE_H = LEFT_PANEL_H - PHONE_Y; // 남은 아래쪽 공간을 전부 사용한단다.

    // [우측 메인 영역: 게임이 실제로 펼쳐지는 곳]
    constexpr int RIGHT_AREA_X = LEFT_PANEL_W; // 왼쪽 패널(40)이 끝나는 지점부터 시작해.
    
    // 뷰포트 (멋진 미소녀 그림이 나오는 커다란 창)
    constexpr int VIEWPORT_X = RIGHT_AREA_X;   
    constexpr int VIEWPORT_Y = 0;              
    constexpr int VIEWPORT_W = CONSOLE_WIDTH - LEFT_PANEL_W; // 전체(160) - 왼쪽(40) = 120칸의 너비야.
    constexpr int VIEWPORT_H = 30;             // 시원하게 30줄을 그림 창으로 써보자.

    // 메시지 로그 (대화가 흐르는 로그 창)
    constexpr int LOG_X = RIGHT_AREA_X;        
    constexpr int LOG_Y = VIEWPORT_H;          // 뷰포트(30줄)가 끝나는 바로 아래부터 시작해.
    constexpr int LOG_W = VIEWPORT_W;          
    constexpr int LOG_H = CONSOLE_HEIGHT - VIEWPORT_H; // 나머지 15줄을 대화창으로 쓴단다.
}

// UI_BRUSH는 우리가 선을 그을 때 쓰는 '붓'의 모양이야.
// 1칸을 꽉 채우는 '█' 문자를 기본으로 사용하기로 했어.
#define UI_BRUSH "█" 
