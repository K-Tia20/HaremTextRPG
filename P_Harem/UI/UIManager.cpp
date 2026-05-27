#include "UIManager.h"
#include <iostream>
#include <windows.h>
#include <sstream>
#include <algorithm>

using namespace UI_LAYOUT;

void UIManager::gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int UIManager::GetVisualLength(const std::string& str) {
    int visualLen = 0;
    bool inAnsi = false;
    for (size_t i = 0; i < str.length(); ) {
        if (str[i] == '\x1b') { inAnsi = true; i++; continue; }
        if (inAnsi) {
            if (str[i] == 'm') inAnsi = false;
            i++; continue;
        }
        if ((unsigned char)str[i] >= 0x80) { 
            int charSize = (((unsigned char)str[i] & 0xF0) == 0xE0) ? 3 : 2;
            visualLen += 2; i += charSize; 
        } else {
            visualLen += 1; i++;
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
    cfi.nFont = 0; cfi.dwFontSize.X = 10; cfi.dwFontSize.Y = 24;
    cfi.FontFamily = FF_DONTCARE; cfi.FontWeight = FW_BOLD;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    COORD bufferSize = { (SHORT)CONSOLE_WIDTH, (SHORT)CONSOLE_HEIGHT };
    SMALL_RECT windowSize = { 0, 0, (SHORT)(CONSOLE_WIDTH - 1), (SHORT)(CONSOLE_HEIGHT - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SetWindowPos(hwnd, HWND_TOP, 50, 50, 1620, 1120, SWP_SHOWWINDOW);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    std::cout << "\x1b[?1049h" << std::flush;
}

void UIManager::RenderMainUI() {
    DrawSolidBox(DATE_X, DATE_Y, DATE_W, DATE_H, "DATE");
    DrawSolidBox(USER_INFO_X, USER_INFO_Y, USER_INFO_W, USER_INFO_H, "STATUS");
    DrawSmartphoneFrame(PHONE_X, PHONE_Y, PHONE_W, PHONE_H);
    DrawSolidBox(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_W, VIEWPORT_H, "WORLD VIEW");
    DrawSolidBox(LOG_X, LOG_Y, LOG_W, LOG_H, "LOG");
    std::cout << std::flush;
}

static std::vector<std::string> g_logs; // 로그 영구 저장을 위한 전역 바구니

void UIManager::ClearLog() {
    g_logs.clear();
    for (int y = LOG_Y + 1; y < LOG_Y + LOG_H - 1; ++y) {
        gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }
    std::cout << std::flush;
}

void UIManager::PrintLog(const std::string& text, bool isOverlap) {
    int maxLineW = LOG_W - 8; // [중요] 여유 공간을 더 확보해서 테두리 보호
    bool isScriptError = (text.find("[Script Error]") != std::string::npos);
    std::stringstream ss(text);
    std::string tempLine;
    int inputPositionX = -1;
    bool firstLineInThisCall = true;

    while (std::getline(ss, tempLine)) {
        if (!tempLine.empty() && tempLine.back() == '\r') tempLine.pop_back();

        std::string currentSubLine = "";
        int currentVisualW = 0;
        
        for (size_t i = 0; i < tempLine.length(); ) {
            // [지능형 태그 체크] 진짜 약속된 태그만 감지해요!
            if (!isScriptError && tempLine[i] == '[') {
                std::string sub = tempLine.substr(i);
                if (sub.find("[INPUT]") == 0 || sub.find("[PLAYER_NAME]") == 0 || sub.find("[HEROINE_NAME]") == 0) {
                    size_t tagEnd = tempLine.find(']', i);
                    if (tagEnd != std::string::npos) {
                        inputPositionX = currentVisualW;
                        i = tagEnd + 1; continue;
                    }
                }
            }

            if (tempLine[i] == '\x1b') {
                size_t colorEnd = tempLine.find('m', i);
                if (colorEnd != std::string::npos) {
                    currentSubLine += tempLine.substr(i, colorEnd - i + 1);
                    i = colorEnd + 1; continue;
                }
            }

            std::string ch;
            int chW = 0;
            if ((unsigned char)tempLine[i] >= 0x80) {
                int size = (((unsigned char)tempLine[i] & 0xF0) == 0xE0) ? 3 : 2;
                ch = tempLine.substr(i, size); chW = 2; i += size;
            } else {
                ch = tempLine.substr(i, 1); chW = 1; i += 1;
            }

            if (currentVisualW + chW > maxLineW) {
                if (isOverlap && firstLineInThisCall && !g_logs.empty()) g_logs.back() = currentSubLine;
                else g_logs.push_back(currentSubLine);
                currentSubLine = ""; currentVisualW = 0; firstLineInThisCall = false;
            }
            currentSubLine += ch; currentVisualW += chW;
        }
        
        if (isOverlap && firstLineInThisCall && !g_logs.empty()) g_logs.back() = currentSubLine;
        else g_logs.push_back(currentSubLine);
        firstLineInThisCall = false;
    }

    if (g_logs.size() > (size_t)(LOG_H - 4)) {
        g_logs.erase(g_logs.begin(), g_logs.begin() + (g_logs.size() - (LOG_H - 4)));
    }

    for (int y = LOG_Y + 1; y < LOG_Y + LOG_H - 1; ++y) {
        gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }

    for (int i = 0; i < (int)g_logs.size(); ++i) {
        int targetY = LOG_Y + 2 + i;
        gotoxy(LOG_X + 3, targetY);
        if (g_logs[i].find("[Script Error]") != std::string::npos) std::cout << "\x1b[31m";
        std::cout << g_logs[i] << "\x1b[0m";
        gotoxy(LOG_X + LOG_W - 1, targetY);
        std::cout << "│";
    }
    
    if (inputPositionX != -1) {
        gotoxy(LOG_X + 3 + inputPositionX, LOG_Y + 2 + (int)g_logs.size() - 1);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    } else {
        gotoxy(LOG_X + LOG_W - 2, LOG_Y + LOG_H - 2);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    }
    std::cout << std::flush;
}

void UIManager::TypeLog(const std::string& text, int delayMs) {
    std::string currentText = "";
    bool inAnsi = false;
    PrintLog(" "); 
    for (size_t i = 0; i < text.length(); ) {
        if (text[i] == '\x1b') { inAnsi = true; currentText += text[i]; i++; continue; }
        if (inAnsi) {
            currentText += text[i];
            if (text[i] == 'm') inAnsi = false;
            i++; continue;
        }
        if ((unsigned char)text[i] >= 0x80) { 
            int charSize = (((unsigned char)text[i] & 0xF0) == 0xE0) ? 3 : 2;
            currentText += text.substr(i, charSize);
            i += charSize;
        } else {
            currentText += text[i];
            i++;
        }
        PrintLog(currentText, true);
        Sleep(delayMs);
    }
}

void UIManager::UpdateDate(const std::string& date) {
    gotoxy(DATE_X + 2, DATE_Y + 1);
    std::cout << std::string(DATE_W - 4, ' ') << std::flush;
    gotoxy(DATE_X + (DATE_W - GetVisualLength(date)) / 2, DATE_Y + 1);
    std::cout << date << std::flush;
}

void UIManager::PrepareInput(const std::string& prompt) {
    if (!prompt.empty()) std::cout << prompt << " " << std::flush;
}

void UIManager::UpdateUserInfo(const std::string& name) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 3);
    std::cout << "👤 NAME : " << name << "          " << std::flush;
}

void UIManager::UpdateMoney(int money) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 5);
    std::cout << "💰 GOLD : " << money << " G            " << std::flush;
}

void UIManager::UpdateHeroineList(const std::vector<HeroineDisplayData>& list) {
    for (int y = PHONE_Y + 2; y < PHONE_Y + PHONE_H - 2; ++y) {
        gotoxy(PHONE_X + 2, y);
        std::cout << std::string(PHONE_W - 4, ' ');
    }
    if (list.empty()) {
        gotoxy(PHONE_X + (PHONE_W - 14) / 2, PHONE_Y + PHONE_H / 2);
        std::cout << "[ LOCKED ]" << std::flush;
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
    std::cout << std::flush;
}

void UIManager::ClearMainViewport() {
    std::string blank(VIEWPORT_W - 2, ' ');
    for (int y = VIEWPORT_Y + 1; y < VIEWPORT_Y + VIEWPORT_H - 1; ++y) {
        gotoxy(VIEWPORT_X + 1, y);
        std::cout << blank;
    }
    std::cout << std::flush;
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
    gotoxy(LOG_X + LOG_W - 2, LOG_Y + LOG_H - 2); 
    std::cout << std::flush;
}
