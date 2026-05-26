#include "UIManager.h"
#include <iostream>
#include <windows.h>

using namespace UI_LAYOUT;

void UIManager::gotoxy(int x, int y) 
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int UIManager::GetVisualLength(const std::string& str) 
{
    int visualLen = 0;
    for (size_t i = 0; i < str.length(); ) {
        if ((unsigned char)str[i] >= 0x80) { 
            visualLen += 2; 
            if (((unsigned char)str[i] & 0xF0) == 0xE0) i += 3; 
            else i += 2; 
        } else {
            visualLen += 1;
            i++;
        }
    }
    return visualLen;
}

void UIManager::DrawSolidBox(int startX, int startY, int width, int height, std::string title) 
{
    // 1. 모서리 출력
    gotoxy(startX, startY); std::cout << "┌";
    gotoxy(startX + width - 1, startY); std::cout << "┐";
    gotoxy(startX, startY + height - 1); std::cout << "└";
    gotoxy(startX + width - 1, startY + height - 1); std::cout << "┘";

    // 2. 가로선 (─)
    for (int x = 1; x < width - 1; x++) {
        gotoxy(startX + x, startY); std::cout << "─";
        gotoxy(startX + x, startY + height - 1); std::cout << "─";
    }

    // 3. 세로선 (│)
    for (int y = 1; y < height - 1; ++y) {
        gotoxy(startX, startY + y); std::cout << "│";
        gotoxy(startX + width - 1, startY + y); std::cout << "│";
    }

    // 4. 타이틀 중앙 정렬 (가로선 위에 덮어쓰기)
    if (!title.empty()) {
        int vLen = GetVisualLength(title) + 2;
        int offset = (width - vLen) / 2;
        if (offset < 1) offset = 1;
        gotoxy(startX + offset, startY);
        std::cout << " " << title << " ";
    }
}

void UIManager::DrawSmartphoneFrame(int startX, int startY, int width, int height)
{
    // 전용 프레임 (스마트폰은 둥근 모서리 느낌을 위해 다른 문자 사용 가능하지만 통일감을 위해 유지)
    DrawSolidBox(startX, startY, width, height, "");
    
    int mid = width / 2;
    // 상단 리시버
    gotoxy(startX + mid - 3, startY); std::cout << "┰╍┰";
    // 하단 버튼
    gotoxy(startX + mid - 2, startY + height - 1); std::cout << "[O]";
}

void UIManager::Init() {
    HWND hwnd = GetConsoleWindow();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, style);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 12;
    cfi.dwFontSize.Y = 24;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_BOLD;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    COORD bufferSize = { (SHORT)CONSOLE_WIDTH, (SHORT)CONSOLE_HEIGHT };
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SMALL_RECT windowSize = { 0, 0, (SHORT)(CONSOLE_WIDTH - 1), (SHORT)(CONSOLE_HEIGHT - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    SetWindowPos(hwnd, HWND_TOP, 100, 100, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    std::cout << "\x1b[?1049h";
}

void UIManager::RenderMainUI() {
    // 1. 날짜 창 (독립 분리)
    DrawSolidBox(DATE_X, DATE_Y, DATE_W, DATE_H, "DATE");

    // 2. 유저 정보 창
    DrawSolidBox(USER_INFO_X, USER_INFO_Y, USER_INFO_W, USER_INFO_H, "STATUS");
    
    // 3. 히로인 목록 (스마트폰 프레임)
    DrawSmartphoneFrame(PHONE_X, PHONE_Y, PHONE_W, PHONE_H);
    
    // 4. 메인 뷰포트
    DrawSolidBox(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_W, VIEWPORT_H, "WORLD VIEW");
    
    // 5. 메시지 로그
    DrawSolidBox(LOG_X, LOG_Y, LOG_W, LOG_H, "LOG");
}

void UIManager::PrintLog(const std::string& text) {
    static std::vector<std::string> logs;
    logs.push_back(text);
    if (logs.size() > (size_t)(LOG_H - 4)) logs.erase(logs.begin());

    for (int i = 0; i < (int)logs.size(); ++i) {
        gotoxy(LOG_X + 3, LOG_Y + 2 + i);
        std::string line = ">> " + logs[i];
        int padding = (LOG_W - 6) - GetVisualLength(line);
        std::cout << line << (padding > 0 ? std::string(padding, ' ') : "");
    }
}

void UIManager::UpdateDate(const std::string& date) {
    gotoxy(DATE_X + (DATE_W - GetVisualLength(date)) / 2, DATE_Y + 1);
    std::cout << date;
}

void UIManager::UpdateUserInfo(const std::string& name) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 3);
    std::cout << "👤 NAME : " << name << "          ";
}

void UIManager::UpdateMoney(int money) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 5);
    std::cout << "💰 GOLD : " << money << " G            ";
}

void UIManager::UpdateHeroineList(const std::vector<HeroineDisplayData>& list) {
    for (int y = PHONE_Y + 2; y < PHONE_Y + PHONE_H - 2; ++y) {
        gotoxy(PHONE_X + 2, y);
        std::cout << std::string(PHONE_W - 4, ' ');
    }

    if (list.empty()) {
        gotoxy(PHONE_X + (PHONE_W - 14) / 2, PHONE_Y + PHONE_H / 2);
        std::cout << "[ LOCKED ]";
        return;
    }

    for (size_t i = 0; i < list.size(); ++i) {
        int baseY = PHONE_Y + 3 + (int)i * 4;
        if (baseY + 2 >= PHONE_Y + PHONE_H - 2) break;

        gotoxy(PHONE_X + 3, baseY);
        std::cout << "♥ " << list[i].name << " (" << list[i].info << ")";
        
        gotoxy(PHONE_X + 3, baseY + 1);
        std::cout << "HP [";
        // 너비 재계산: PHONE_W(40) - 여백(3) - "HP ["(4) - "] 100/100"(최대 8) = 약 25?
        // 테두리를 뚫지 않도록 확실하게 15칸으로 제한합니다.
        int barW = 15; 
        int hpBar = (list[i].maxHp > 0) ? (list[i].hp * barW / list[i].maxHp) : 0;
        for (int b = 0; b < barW; ++b) {
            if (b < hpBar) std::cout << "\x1b[92m■\x1b[0m";
            else std::cout << "\x1b[90m□\x1b[0m";
        }
        std::cout << "] " << list[i].hp << "/" << list[i].maxHp;
    }
}

void UIManager::ClearMainViewport() {
    std::string blank(VIEWPORT_W - 2, ' ');
    for (int y = VIEWPORT_Y + 1; y < VIEWPORT_Y + VIEWPORT_H - 1; ++y) {
        gotoxy(VIEWPORT_X + 1, y);
        std::cout << blank;
    }
}

void UIManager::DrawImage(const std::string& imageAnsi) {
    // 뷰포트 내부 시작 좌표 (테두리 안쪽)
    int startX = VIEWPORT_X + 1;
    int startY = VIEWPORT_Y + 1;
    
    int currentY = startY;
    gotoxy(startX, currentY);

    for (size_t i = 0; i < imageAnsi.length(); ++i) {
        if (imageAnsi[i] == '\n') {
            currentY++;
            if (currentY >= VIEWPORT_Y + VIEWPORT_H - 1) break;
            gotoxy(startX, currentY);
        } else {
            std::cout << imageAnsi[i];
        }
    }
}
