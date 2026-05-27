#pragma once

#include <string>
#include <vector>
#include "UILayoutDefs.h"

/**
 * @class UIManager
 * @brief 콘솔 렌더링 및 레이아웃 제어를 담당하는 핵심 클래스입니다.
 */
class UIManager {
private:
    int GetVisualLength(const std::string& str);
    void DrawSolidBox(int startX, int startY, int width, int height, std::string title);
    void DrawSmartphoneFrame(int startX, int startY, int width, int height);
    void gotoxy(int x, int y);

    std::string m_currentPrompt = ">> ";

public:
    void Init(); 
    void RenderMainUI(); 

    // Update APIs
    void PrintLog(const std::string& text, bool isOverlap = false); 
    void TypeLog(const std::string& text, int delayMs = 30); 
    void UpdateMoney(int money);
    void UpdateDate(const std::string& date);
    void UpdateUserInfo(const std::string& name);
    
    struct HeroineDisplayData {
        std::string name;
        int level;
        int hp;
        int maxHp;
        int attack;
        int affinity;
        std::string style;
    };

    void UpdateHeroineList(const std::vector<HeroineDisplayData>& list);
    void ClearMainViewport();
    void ClearLog(); 
    void DrawImage(const std::string& imageAnsi);
    void DrawInputBox(const std::string& prompt = ""); 
    void SetCursorToInputArea(const std::string& prompt = ""); 

    // Cinematic Animations
    void CenteredTypeLog(const std::string& text, int y, int delayMs = 100);
    void PlayBlueTransition();

    std::string GetCurrentPrompt() const { return m_currentPrompt; }
};
