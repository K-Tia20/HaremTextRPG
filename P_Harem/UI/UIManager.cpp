#include "UIManager.h"
#include <iostream>
#include <windows.h>
#include <sstream>
#include <algorithm>

using namespace UI_LAYOUT;

/**
 * [gotoxy] - 절대 방어 버전
 * 화면의 맨 마지막 줄(44)과 맨 오른쪽 칸(159)은 절대 밟지 않습니다.
 */
void UIManager::gotoxy(int x, int y) {
    if (x < 0) x = 0; if (y < 0) y = 0;
    // 160x45 콘솔에서 159와 44는 스크롤 유발 지점이므로 1칸씩 안쪽으로 제한합니다.
    if (x >= CONSOLE_WIDTH - 1) x = CONSOLE_WIDTH - 2;
    if (y >= CONSOLE_HEIGHT - 1) y = CONSOLE_HEIGHT - 2;

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
    int endX = startX + width - 1;
    int endY = startY + height - 1;

    // 테두리 좌표가 세이프 존을 넘지 않도록 강제 보정
    if (endX >= CONSOLE_WIDTH - 1) endX = CONSOLE_WIDTH - 2;
    if (endY >= CONSOLE_HEIGHT - 1) endY = CONSOLE_HEIGHT - 2;

    gotoxy(startX, startY); std::cout << "┌";
    gotoxy(endX, startY); std::cout << "┐";
    gotoxy(startX, endY); std::cout << "└";
    gotoxy(endX, endY); std::cout << "┘";

    for (int x = startX + 1; x < endX; x++) {
        gotoxy(x, startY); std::cout << "─";
        gotoxy(x, endY); std::cout << "─";
    }
    for (int y = startY + 1; y < endY; ++y) {
        gotoxy(startX, y); std::cout << "│";
        gotoxy(endX, y); std::cout << "│";
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
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_BOLD;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    // [절대 매칭] 버퍼 == 창크기 (160x45)
    COORD bufferSize = { (SHORT)CONSOLE_WIDTH, (SHORT)CONSOLE_HEIGHT };
    SMALL_RECT windowSize = { 0, 0, (SHORT)(CONSOLE_WIDTH - 1), (SHORT)(CONSOLE_HEIGHT - 1) };
    
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    SetWindowPos(hwnd, HWND_TOP, 50, 50, 1620, 1120, SWP_SHOWWINDOW);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    dwMode &= ~ENABLE_WRAP_AT_EOL_OUTPUT; // 자동 줄바꿈 완전 차단
    SetConsoleMode(hOut, dwMode);

    std::cout << "\x1b[?1049h" << std::flush;
}

void UIManager::RenderMainUI() {
    DrawSolidBox(DATE_X, DATE_Y, DATE_W, DATE_H, "DATE");
    DrawSolidBox(USER_INFO_X, USER_INFO_Y, USER_INFO_W, USER_INFO_H, "STATUS");
    DrawSmartphoneFrame(PHONE_X, PHONE_Y, PHONE_W, PHONE_H);
    DrawInputBox(); 
    DrawSolidBox(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_W, VIEWPORT_H, "WORLD VIEW");
    DrawSolidBox(LOG_X, LOG_Y, LOG_W, LOG_H, "LOG");
    std::cout << std::flush;
}

static std::vector<std::string> g_logs; 
static int g_lastAddedLines = 0; 

void UIManager::ClearLog() {
    g_logs.clear();
    g_lastAddedLines = 0;
    for (int y = LOG_Y + 1; y < LOG_Y + LOG_H - 1; ++y) {
        gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }
    std::cout << std::flush;
}

void UIManager::DrawInputBox(const std::string& prompt) {
    DrawSolidBox(INPUT_BOX_X, INPUT_BOX_Y, INPUT_BOX_W, INPUT_BOX_H, "INPUT");
    std::string cleaner(INPUT_BOX_W - 2, ' ');
    for (int y = INPUT_BOX_Y + 1; y < INPUT_BOX_Y + INPUT_BOX_H - 1; ++y) {
        gotoxy(INPUT_BOX_X + 1, y);
        std::cout << cleaner;
    }
    if (!prompt.empty()) {
        gotoxy(INPUT_BOX_X + 2, INPUT_BOX_Y + 1);
        std::cout << "\x1b[96m" << prompt << "\x1b[0m";
    }
    std::cout << std::flush;
}

void UIManager::SetCursorToInputArea(const std::string& prompt) {
    if (!prompt.empty()) m_currentPrompt = prompt;
    DrawInputBox(m_currentPrompt);
    gotoxy(INPUT_BOX_X + 2, INPUT_BOX_Y + 2);
    CONSOLE_CURSOR_INFO ci = { 100, TRUE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    std::cout << "\x1b[93m>> \x1b[0m" << std::flush;
}

void UIManager::PrintLog(const std::string& text, bool isOverlap) {
    int maxLogLines = LOG_H - 4; 
    int maxLineW = LOG_W - 10;
    std::string processText = text;
    
    size_t pipePos = processText.find('|');
    if (pipePos != std::string::npos) {
        std::string promptMsg = processText.substr(pipePos + 1);
        size_t inputTag = promptMsg.find("[INPUT]");
        if (inputTag != std::string::npos) promptMsg.erase(inputTag, 7);
        m_currentPrompt = promptMsg; 
        processText = processText.substr(0, pipePos); 
    } else if (!isOverlap) {
        m_currentPrompt = ">> ";
    }

    if (isOverlap && g_lastAddedLines > 0) {
        for (int i = 0; i < g_lastAddedLines; ++i) {
            if (!g_logs.empty()) g_logs.pop_back();
        }
    }

    std::stringstream ss(processText);
    std::string tempLine;
    static std::string lastAnsiState = ""; 
    int currentCallLines = 0;

    while (std::getline(ss, tempLine)) {
        if (!tempLine.empty() && tempLine.back() == '\r') tempLine.pop_back();
        std::string currentSubLine = lastAnsiState; 
        int currentVisualW = 0;
        for (size_t i = 0; i < tempLine.length(); ) {
            if (tempLine[i] == '\x1b') {
                size_t colorEnd = tempLine.find('m', i);
                if (colorEnd != std::string::npos) {
                    std::string ansi = tempLine.substr(i, colorEnd - i + 1);
                    lastAnsiState = (ansi == "\x1b[0m") ? "" : ansi;
                    currentSubLine += ansi;
                    i = colorEnd + 1; continue;
                }
            }
            std::string ch; int chW = 0;
            if ((unsigned char)tempLine[i] >= 0x80) {
                int size = (((unsigned char)tempLine[i] & 0xF0) == 0xE0) ? 3 : 2;
                ch = tempLine.substr(i, size); chW = 2; i += size;
            } else {
                ch = tempLine.substr(i, 1); chW = 1; i += 1;
            }
            if (currentVisualW + chW > maxLineW) {
                g_logs.push_back(currentSubLine + "\x1b[0m");
                currentCallLines++;
                currentSubLine = lastAnsiState + ch; currentVisualW = chW; 
            } else {
                currentSubLine += ch; currentVisualW += chW;
            }
        }
        g_logs.push_back(currentSubLine);
        currentCallLines++;
    }

    if (isOverlap) g_lastAddedLines = currentCallLines;
    else g_lastAddedLines = 0;

    if (g_logs.size() > (size_t)maxLogLines) {
        g_logs.erase(g_logs.begin(), g_logs.begin() + (g_logs.size() - maxLogLines));
    }

    for (int y = LOG_Y + 1; y < LOG_Y + LOG_H - 1; ++y) {
        gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }
    for (int i = 0; i < (int)g_logs.size(); ++i) {
        int targetY = LOG_Y + 2 + i;
        gotoxy(LOG_X + 3, targetY);
        std::cout << g_logs[i] << "\x1b[0m";
        gotoxy(LOG_X + LOG_W - 1, targetY);
        std::cout << "│";
    }
    
    // [중요] 커서를 스크롤 위험 구역(44번 행)에서 멀리 떨어진 1번 행으로 대피
    gotoxy(CONSOLE_WIDTH - 2, 1); 
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    std::cout << std::flush;
}

void UIManager::TypeLog(const std::string& text, int delayMs) {
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::string currentText = "";
        g_lastAddedLines = 0; 
        for (size_t i = 0; i < line.length(); ) {
            if (line[i] == '\x1b') {
                size_t mPos = line.find('m', i);
                if (mPos != std::string::npos) {
                    currentText += line.substr(i, mPos - i + 1);
                    i = mPos + 1; continue; 
                }
            }
            if ((unsigned char)line[i] >= 0x80) { 
                int charSize = (((unsigned char)line[i] & 0xF0) == 0xE0) ? 3 : 2;
                currentText += line.substr(i, charSize);
                i += charSize;
            } else {
                currentText += line[i]; i++;
            }
            PrintLog(currentText, true);
            Sleep(delayMs);
        }
    }
}

void UIManager::CenteredTypeLog(const std::string& text, int y, int delayMs) {
    int startX = (CONSOLE_WIDTH - GetVisualLength(text)) / 2;
    std::string current = "";
    for (size_t i = 0; i < text.length(); ) {
        if ((unsigned char)text[i] >= 0x80) {
            int size = (((unsigned char)text[i] & 0xF0) == 0xE0) ? 3 : 2;
            current += text.substr(i, size); i += size;
        } else {
            current += text[i]; i++;
        }
        gotoxy(startX, y);
        std::cout << current << std::flush;
        Sleep(delayMs);
    }
}

void UIManager::PlayBlueTransition() {
    // 사용자님 요청에 따라 더 구린 파란색 그라데이션 대신 
    // 깔끔한 화면 청소 후 이미지를 띄울 준비를 합니다.
    system("cls");
}

void UIManager::UpdateDate(const std::string& date) {
    gotoxy(DATE_X + 2, DATE_Y + 1);
    std::cout << std::string(DATE_W - 4, ' ') << std::flush;
    gotoxy(DATE_X + (DATE_W - GetVisualLength(date)) / 2, DATE_Y + 1);
    std::cout << date << std::flush;
}

void UIManager::UpdateUserInfo(const std::string& name) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 2);
    std::cout << "👤 이  름 : " << name << "          " << std::flush;
}

void UIManager::UpdateMoney(int money) {
    gotoxy(USER_INFO_X + 4, USER_INFO_Y + 4);
    std::cout << "💰 소지금 : " << money << " 원            " << std::flush;
}

void UIManager::UpdateHeroineList(const std::vector<HeroineDisplayData>& list) {
    for (int y = PHONE_Y + 1; y < PHONE_Y + PHONE_H - 1; ++y) {
        gotoxy(PHONE_X + 2, y);
        std::cout << std::string(PHONE_W - 4, ' ');
    }
    if (list.empty()) {
        gotoxy(PHONE_X + (PHONE_W - 14) / 2, PHONE_Y + PHONE_H / 2);
        std::cout << "\x1b[90m[ 연락처 없음 ]\x1b[0m";
        return;
    }
    for (size_t i = 0; i < list.size(); ++i) {
        int baseY = PHONE_Y + 1 + (int)i * 6;
        if (baseY + 5 >= PHONE_Y + PHONE_H - 1) break;
        const auto& d = list[i];
        std::string styleColor = "\x1b[37m";
        if (d.style.find("핫걸") != std::string::npos) styleColor = "\x1b[31m";
        else if (d.style.find("냉미녀") != std::string::npos) styleColor = "\x1b[36m";
        else if (d.style.find("초식녀") != std::string::npos) styleColor = "\x1b[32m";
        
        gotoxy(PHONE_X + 3, baseY);
        std::cout << styleColor << "♥ " << d.name << " (" << d.style << ")\x1b[0m";
        gotoxy(PHONE_X + 3, baseY + 1);
        std::cout << "\x1b[93m등급: Lv." << d.level << "\x1b[0m | \x1b[91m매력: " << d.attack << "\x1b[0m";
        gotoxy(PHONE_X + 3, baseY + 2);
        std::cout << "체력 ["; 
        int hpBarW = 15;
        int hpGage = (d.maxHp > 0) ? (d.hp * hpBarW / d.maxHp) : 0;
        for (int b = 0; b < hpBarW; ++b) {
            if (b < hpGage) std::cout << "\x1b[92m■\x1b[0m";
            else std::cout << "\x1b[90m□\x1b[0m";
        }
        std::cout << "] " << d.hp << "/" << d.maxHp;
        gotoxy(PHONE_X + 3, baseY + 3);
        std::cout << "애정 ["; 
        int afBarW = 15;
        int afGage = (d.affinity * afBarW / 100);
        if (afGage > afBarW) afGage = afBarW;
        for (int b = 0; b < afBarW; ++b) {
            if (b < afGage) std::cout << "\x1b[95m■\x1b[0m";
            else std::cout << "\x1b[90m□\x1b[0m";
        }
        std::cout << "] " << d.affinity << "%";
        gotoxy(PHONE_X + 3, baseY + 4);
        std::cout << "\x1b[90m----------------------------\x1b[0m";
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
    gotoxy(CONSOLE_WIDTH - 2, 1);
    std::cout << std::flush;
}
