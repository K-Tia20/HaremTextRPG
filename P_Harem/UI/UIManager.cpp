#include "UIManager.h"
#include <iostream>
#include <windows.h>
#include <sstream>

using namespace UI_LAYOUT;

void UIManager::gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int UIManager::GetVisualLength(const std::string& str) {
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

void UIManager::DrawSolidBox(int startX, int startY, int width, int height, std::string title) {
    gotoxy(startX, startY); std::cout << "┌";
    gotoxy(startX + width - 1, startY); std::cout << "┐";
    gotoxy(startX, startY + height - 1); std::cout << "└";
    gotoxy(startX + width - 1, startY + height - 1); std::cout << "┘";

    for (int x = 1; x < width - 1; x++) {
        gotoxy(startX + x, startY); std::cout << "─";
        gotoxy(startX + x, startY + height - 1); std::cout << "─";
    }
    for (int y = 1; y < height - 1; ++y) {
        gotoxy(startX, startY + y); std::cout << "│";
        gotoxy(startX + width - 1, startY + y); std::cout << "│";
    }

    if (!title.empty()) {
        int vLen = GetVisualLength(title) + 2;
        int offset = (width - vLen) / 2;
        if (offset < 1) offset = 1;
        gotoxy(startX + offset, startY);
        std::cout << " " << title << " ";
    }
}

void UIManager::DrawSmartphoneFrame(int startX, int startY, int width, int height) {
    DrawSolidBox(startX, startY, width, height, "");
    int mid = width / 2;
    gotoxy(startX + mid - 3, startY); std::cout << "┰╍┰";
    gotoxy(startX + mid - 2, startY + height - 1); std::cout << "[O]";
}

void UIManager::Init() {
    HWND hwnd = GetConsoleWindow();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, style);

    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
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
    DrawSolidBox(DATE_X, DATE_Y, DATE_W, DATE_H, "DATE");
    DrawSolidBox(USER_INFO_X, USER_INFO_Y, USER_INFO_W, USER_INFO_H, "STATUS");
    DrawSmartphoneFrame(PHONE_X, PHONE_Y, PHONE_W, PHONE_H);
    DrawSolidBox(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_W, VIEWPORT_H, "WORLD VIEW");
    DrawSolidBox(LOG_X, LOG_Y, LOG_W, LOG_H, "LOG");
}

void UIManager::PrintLog(const std::string& text) {
    static std::vector<std::string> logs;
    logs.push_back(text);
    if (logs.size() > (size_t)(LOG_H - 4)) logs.erase(logs.begin());

    for (int i = 0; i < (int)logs.size(); ++i) {
        int targetY = LOG_Y + 2 + i;
        gotoxy(LOG_X + 3, targetY);
        std::string line = ">> " + logs[i];
        
        int maxInnerW = LOG_W - 6; 
        int padding = maxInnerW - GetVisualLength(line);
        
        std::cout << line;
        if (padding > 0) std::cout << std::string(padding, ' ');

        gotoxy(LOG_X + LOG_W - 1, targetY);
        std::cout << "│";
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
    if (imageAnsi.empty()) return;
    int startX = VIEWPORT_X + 1;
    int startY = VIEWPORT_Y + 1;
    
    int lineCount = 0;
    for (char c : imageAnsi) if (c == '\n') lineCount++;
    
    int yOffset = ( (VIEWPORT_H - 2) - lineCount ) / 2;
    if (yOffset < 0) yOffset = 0;
    
    int currentY = startY + yOffset;
    std::string line;
    std::stringstream ss(imageAnsi);
    while (std::getline(ss, line)) {
        if (currentY >= VIEWPORT_Y + VIEWPORT_H - 1) break;
        gotoxy(startX, currentY++);
        std::cout << line;
    }
    gotoxy(0, CONSOLE_HEIGHT - 1);
}
