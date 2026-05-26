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

public:
    void Init(); 
    void RenderMainUI(); 

    // Update APIs
    void PrintLog(const std::string& text); 
    void UpdateMoney(int money);
    void UpdateDate(const std::string& date);
    void UpdateUserInfo(const std::string& name);
    
    struct HeroineDisplayData {
        std::string name;
        int hp;
        int maxHp;
        std::string info;
    };

    void UpdateHeroineList(const std::vector<HeroineDisplayData>& list);
    void ClearMainViewport();
    void DrawImage(const std::string& imageAnsi);
};
