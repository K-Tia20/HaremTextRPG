#include "UIManager.h"
#include <iostream>
#include <windows.h>
#include <sstream>
#include <algorithm>
#include <conio.h>

using namespace UI_LAYOUT;

// 지정 좌표로 커서 이동 (44번 줄 데드존 보호 로직 포함)
void UIManager::gotoxy(int x, int y) {
    if (x < 0) x = 0; if (y < 0) y = 0;
    if (x > CONSOLE_WIDTH - 1) x = CONSOLE_WIDTH - 1;
    if (y > CONSOLE_HEIGHT - 2) y = CONSOLE_HEIGHT - 2;

    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ANSI 코드를 제외한 실제 글자 너비 계산
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

// 테두리 박스 그리기
void UIManager::DrawSolidBox(int startX, int startY, int width, int height, std::string title) {
    int endX = startX + width - 1;
    int endY = startY + height - 1;

    if (endX > CONSOLE_WIDTH - 1) endX = CONSOLE_WIDTH - 1;
    if (endY > CONSOLE_HEIGHT - 1) endY = CONSOLE_HEIGHT - 1;

    this->gotoxy(startX, startY); std::cout << "┌";
    this->gotoxy(endX, startY); std::cout << "┐";
    this->gotoxy(startX, endY); std::cout << "└";
    this->gotoxy(endX, endY); std::cout << "┘";

    for (int x = startX + 1; x < endX; x++) {
        this->gotoxy(x, startY); std::cout << "─";
        this->gotoxy(x, endY); std::cout << "─";
    }
    for (int y = startY + 1; y < endY; ++y) {
        this->gotoxy(startX, y); std::cout << "│";
        this->gotoxy(endX, y); std::cout << "│";
    }

    if (!title.empty()) {
        int vLen = GetVisualLength(title) + 2;
        int offset = (width - vLen) / 2;
        if (offset < 1) offset = 1;
        this->gotoxy(startX + offset, startY);
        std::cout << " " << title << " ";
    }
}

// 스마트폰 프레임 디자인 추가
void UIManager::DrawSmartphoneFrame(int startX, int startY, int width, int height) {
    DrawSolidBox(startX, startY, width, height, ""); 
    int mid = width / 2;
    this->gotoxy(startX + mid - 3, startY); std::cout << "┰╍┰";
    this->gotoxy(startX + mid - 2, startY + height - 1); std::cout << "[O]";
}

// DPI 인식 및 해상도별 폰트 스케일링 초기화
void UIManager::Init() {
    SetProcessDPIAware();
    HWND hwnd = GetConsoleWindow();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int fontSizeY = 24; 

    if (screenHeight < 1080) fontSizeY = 18;
    if (screenHeight < 900)  fontSizeY = 16;
    if (screenHeight < 720)  fontSizeY = 14; 

    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, style);

    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontSizeY / 2; 
    cfi.dwFontSize.Y = fontSizeY;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_BOLD;
    wcscpy_s(cfi.FaceName, L"Consolas"); 
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    COORD bufferSize = { (SHORT)CONSOLE_WIDTH, (SHORT)CONSOLE_HEIGHT };
    SMALL_RECT windowSize = { 0, 0, (SHORT)(CONSOLE_WIDTH - 1), (SHORT)(CONSOLE_HEIGHT - 1) };
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    int winW = fontSizeY / 2 * CONSOLE_WIDTH + 100;
    int winH = fontSizeY * CONSOLE_HEIGHT + 100;
    SetWindowPos(hwnd, HWND_TOP, (GetSystemMetrics(SM_CXSCREEN) - winW) / 2, (GetSystemMetrics(SM_CYSCREEN) - winH) / 2, winW, winH, SWP_SHOWWINDOW);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    dwMode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;        
    SetConsoleMode(hOut, dwMode);

    std::cout << "\x1b[?1049h" << std::flush;
}

// 메인 프레임워크 렌더링
void UIManager::RenderMainUI() {
    system("cls"); 
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

// 로그창 비우기
void UIManager::ClearLog() {
    g_logs.clear();
    g_lastAddedLines = 0;
    for (int y = LOG_Y + 1; y < LOG_Y + LOG_H - 1; ++y) {
        this->gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }
    std::cout << std::flush;
}

// 입력창 프롬프트 갱신
void UIManager::DrawInputBox(const std::string& prompt) {
    DrawSolidBox(INPUT_BOX_X, INPUT_BOX_Y, INPUT_BOX_W, INPUT_BOX_H, "INPUT");
    std::string cleaner(INPUT_BOX_W - 2, ' ');
    for (int y = INPUT_BOX_Y + 1; y < INPUT_BOX_Y + INPUT_BOX_H - 1; ++y) {
        this->gotoxy(INPUT_BOX_X + 1, y);
        std::cout << cleaner;
    }
    if (!prompt.empty()) {
        this->gotoxy(INPUT_BOX_X + 2, INPUT_BOX_Y + 1);
        std::cout << "\x1b[96m" << prompt << "\x1b[0m";
    }
    std::cout << std::flush;
}

// 입력 대기 모드로 커서 이동
void UIManager::SetCursorToInputArea(const std::string& prompt) {
    if (!prompt.empty()) m_currentPrompt = prompt;
    DrawInputBox(m_currentPrompt);
    this->gotoxy(INPUT_BOX_X + 2, INPUT_BOX_Y + 2);
    CONSOLE_CURSOR_INFO ci = { 100, TRUE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    std::cout << "\x1b[93m>> \x1b[0m" << std::flush;
}

// 로그 출력 및 자동 스크롤 관리
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
        this->gotoxy(LOG_X + 2, y);
        std::cout << std::string(LOG_W - 4, ' ');
    }
    for (int i = 0; i < (int)g_logs.size(); ++i) {
        int targetY = LOG_Y + 2 + i;
        this->gotoxy(LOG_X + 3, targetY);
        std::cout << g_logs[i] << "\x1b[0m"; 
        this->gotoxy(LOG_X + LOG_W - 1, targetY);  
        std::cout << "│";
    }
    
    this->gotoxy(CONSOLE_WIDTH - 2, 1); 
    CONSOLE_CURSOR_INFO ci = { 1, FALSE }; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    std::cout << std::flush;
}

// 타자기 연출
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
                if (mPos != std::string::npos) { currentText += line.substr(i, mPos - i + 1); i = mPos + 1; continue; }
            }
            if ((unsigned char)line[i] >= 0x80) { 
                int charSize = (((unsigned char)line[i] & 0xF0) == 0xE0) ? 3 : 2;
                currentText += line.substr(i, charSize); i += charSize;
            } else {
                currentText += line[i]; i++;
            }
            PrintLog(currentText, true);
            Sleep(delayMs); 
        }
    }
}

// 히로인 리스트 렌더링 (startIndex 기반 스크롤 지원)
void UIManager::UpdateHeroineList(const std::vector<HeroineDisplayData>& list, int startIndex) {
    for (int y = PHONE_Y + 1; y < PHONE_Y + PHONE_H - 1; ++y) {
        this->gotoxy(PHONE_X + 2, y);
        std::cout << std::string(PHONE_W - 4, ' ');
    }
    if (list.empty()) {
        this->gotoxy(PHONE_X + (PHONE_W - 14) / 2, PHONE_Y + PHONE_H / 2);
        std::cout << "\x1b[90m[ 연락처 없음 ]\x1b[0m";
        return;
    }

    int displayLimit = 4; 
    int count = 0;

    for (size_t i = (size_t)startIndex; i < list.size() && count < displayLimit; ++i) {
        int baseY = PHONE_Y + 1 + count * 6;
        const auto& d = list[i];
        
        std::string styleColor = "\x1b[37m";
        if (d.style.find("핫걸") != std::string::npos) styleColor = "\x1b[31m";
        else if (d.style.find("냉미녀") != std::string::npos) styleColor = "\x1b[36m";
        else if (d.style.find("초식녀") != std::string::npos) styleColor = "\x1b[32m";
        
        this->gotoxy(PHONE_X + 3, baseY);
        std::cout << styleColor << "♥ " << d.name << " (" << d.style << ")\x1b[0m";
        this->gotoxy(PHONE_X + 3, baseY + 1);
        std::cout << "\x1b[93m등급: Lv." << d.level << "\x1b[0m | \x1b[91m매력: " << d.attack << "\x1b[0m";
        
        this->gotoxy(PHONE_X + 3, baseY + 2);
        std::cout << "체력 ["; 
        int hpBarW = 15;
        int hpGage = (d.maxHp > 0) ? (d.hp * hpBarW / d.maxHp) : 0;
        for (int b = 0; b < hpBarW; ++b) std::cout << (b < hpGage ? "\x1b[92m■\x1b[0m" : "\x1b[90m□\x1b[0m");
        std::cout << "] " << d.hp << "/" << d.maxHp;

        this->gotoxy(PHONE_X + 3, baseY + 3);
        std::cout << "애정 ["; 
        int afBarW = 15;
        int afGage = (d.affinity * afBarW / 100);
        if (afGage > afBarW) afGage = afBarW;
        for (int b = 0; b < afBarW; ++b) std::cout << (b < afGage ? "\x1b[95m■\x1b[0m" : "\x1b[90m□\x1b[0m");
        std::cout << "] " << d.affinity << "%";
        
        this->gotoxy(PHONE_X + 3, baseY + 4);
        std::cout << "\x1b[90m----------------------------\x1b[0m";
        count++;
    }

    if (list.size() > (size_t)displayLimit) {
        this->gotoxy(PHONE_X + 10, PHONE_Y + PHONE_H - 2);
        std::cout << "\x1b[93m... 더 보기 (▲▼)\x1b[0m";
    }
    std::cout << std::flush;
}

// 인벤토리 리스트 렌더링 (startIndex 기반 스크롤 지원)
void UIManager::UpdateInventoryList(const std::vector<ItemDisplayData>& list, int startIndex) {
    for (int y = PHONE_Y + 1; y < PHONE_Y + PHONE_H - 1; ++y) {
        this->gotoxy(PHONE_X + 2, y);
        std::cout << std::string(PHONE_W - 4, ' ');
    }
    if (list.empty()) {
        this->gotoxy(PHONE_X + (PHONE_W - 14) / 2, PHONE_Y + PHONE_H / 2);
        std::cout << "\x1b[90m[ 가방이 비어있음 ]\x1b[0m";
        return;
    }

    int displayLimit = 8; 
    int count = 0;

    for (size_t i = (size_t)startIndex; i < list.size() && count < displayLimit; ++i) {
        int baseY = PHONE_Y + 1 + count * 3;
        const auto& item = list[i];
        this->gotoxy(PHONE_X + 3, baseY);
        std::cout << "\x1b[97m" << item.name << "\x1b[0m \x1b[93m(x" << item.quantity << ")\x1b[0m";
        this->gotoxy(PHONE_X + 3, baseY + 1);
        std::cout << "\x1b[90m└ " << item.typeStr << " 효과: " << item.value << "\x1b[0m";
        count++;
    }

    if (list.size() > (size_t)displayLimit) {
        this->gotoxy(PHONE_X + 10, PHONE_Y + PHONE_H - 2);
        std::cout << "\x1b[93m... 더 보기 (▲▼)\x1b[0m";
    }
    std::cout << std::flush;
}

// 레벨업 이벤트 팝업
void UIManager::ShowLevelUpEvent(const std::string& name, int level) {
    int centerX = VIEWPORT_X + (VIEWPORT_W / 2);
    int centerY = VIEWPORT_Y + (VIEWPORT_H / 2);
    int boxW = 50; int boxH = 7;
    int startX = centerX - (boxW / 2);
    int startY = centerY - (boxH / 2);
    
    DrawSolidBox(startX, startY, boxW, boxH, " LEVEL UP!! ");
    this->gotoxy(startX + 5, startY + 2);
    std::cout << "\x1b[93m★축하합니다!★\x1b[0m";
    this->gotoxy(startX + 5, startY + 4);
    std::cout << "\x1b[97m[" << name << "]\x1b[0m 님의 등급이 \x1b[92mLv." << level << "\x1b[0m 로 상승!";
    std::cout << std::flush;
}

// 메인 뷰포트 영역 청소
void UIManager::ClearMainViewport() {
    std::string blank(VIEWPORT_W - 2, ' ');
    for (int y = VIEWPORT_Y + 1; y < VIEWPORT_Y + VIEWPORT_H - 1; ++y) {
        this->gotoxy(VIEWPORT_X + 1, y);
        std::cout << blank;
    }
    std::cout << std::flush;
}

// 중앙 정렬 타이핑 효과
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
        this->gotoxy(startX, y);
        std::cout << current << std::flush;
        Sleep(delayMs);
    }
}

// 화면 전환 트랜지션
void UIManager::PlayBlueTransition() { system("cls"); }

// 정보창 갱신 함수들
void UIManager::UpdateDate(const std::string& date) {
    this->gotoxy(DATE_X + 2, DATE_Y + 1);
    std::cout << std::string(DATE_W - 4, ' ') << std::flush; 
    this->gotoxy(DATE_X + (DATE_W - GetVisualLength(date)) / 2, DATE_Y + 1);
    std::cout << date << std::flush; 
}

void UIManager::UpdateUserInfo(const std::string& name) {
    this->gotoxy(USER_INFO_X + 4, USER_INFO_Y + 2);
    std::cout << "👤 이  름 : " << name << "          " << std::flush;
}

void UIManager::UpdateMoney(int money) {
    this->gotoxy(USER_INFO_X + 4, USER_INFO_Y + 4);
    std::cout << "💰 소지금 : " << money << " 원            " << std::flush;
}

// 뷰포트 이미지 렌더링 (세로 중앙 정렬)
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
        this->gotoxy(startX, currentY++);
        std::cout << line;
    }
    this->gotoxy(CONSOLE_WIDTH - 2, 1); 
    std::cout << std::flush;
}

// 콘솔 전체 중앙 이미지 렌더링
void UIManager::DrawImageAtCenter(const std::string& imageAnsi) {
    if (imageAnsi.empty()) return;
    int imgW = VIEWPORT_W - 2; 
    int imgH = VIEWPORT_H - 2; 
    int startX = (CONSOLE_WIDTH - imgW) / 2;
    int startY = (CONSOLE_HEIGHT - imgH) / 2;
    int currentY = startY;
    std::string line;
    std::stringstream ss(imageAnsi);
    while (std::getline(ss, line)) {
        if (currentY >= CONSOLE_HEIGHT - 2) break;
        this->gotoxy(startX, currentY++);
        std::cout << line;
    }
    this->gotoxy(CONSOLE_WIDTH - 2, 1);
    std::cout << std::flush;
}

// 키 대기 유틸리티
void UIManager::WaitKey(UIManager* ui) {
    if (!ui) return;
    ui->PrintLog("\x1b[90m[ 엔터를 눌러 계속... ]\x1b[0m");
    std::string dummy;
    std::getline(std::cin, dummy);
    ui->ClearLog();
}

int UIManager::GetInputInt() {
    std::string input = "";
    
    // 쌓인 엔터 찌꺼기 청소
    while (_kbhit()) _getch();

    while (true) {
        int ch = _getch();

        if (ch == '\r') { 
            break;
        }
        else if (ch == '\b') { 
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        }
        else if (isdigit(ch)) {
            input += (char)ch;
            std::cout << (char)ch << std::flush;
        }
    }
    
    if (input.empty()) return -1;
    return std::stoi(input);
}