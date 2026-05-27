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
    void PrintLog(const std::string& text, bool isOverlap = false); 
    void TypeLog(const std::string& text, int delayMs = 30); 
 // 타이핑 효과를 주는 로그 출력
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
    void ClearLog(); // 로그창의 모든 기록을 지우고 깨끗하게 만듭니다.
    void DrawImage(const std::string& imageAnsi);

    /**
     * @brief 로그창 내부의 입력 전용 위치로 커서를 이동시키고 안내 문구를 띄웁니다.
     */
    void PrepareInput(const std::string& prompt);
};
