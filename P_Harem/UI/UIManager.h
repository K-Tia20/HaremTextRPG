#pragma once

#include <string>
#include <vector>
#include "UILayoutDefs.h"

/**
 * @class UIManager
 * @brief 콘솔 렌더링 엔진 및 레이아웃 관리자
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
    int GetInputInt();
    
    // 로그 및 타이핑 연출
    void PrintLog(const std::string& text, bool isOverlap = false); 
    void TypeLog(const std::string& text, int delayMs = 30); 
    
    // 상태창 데이터 갱신 API
    void UpdateMoney(int money);
    void UpdateDate(const std::string& date);
    void UpdateUserInfo(const std::string& name);
    
    struct HeroineDisplayData {
        std::string name; int level; int hp; int maxHp; int attack; int affinity; std::string style;
    };

    struct ItemDisplayData {
        std::string name; int quantity; int value; std::string typeStr;
    };

    // 리스트 출력 (스크롤 지원용 startIndex 추가)
    void UpdateHeroineList(const std::vector<HeroineDisplayData>& list, int startIndex = 0);
    void UpdateInventoryList(const std::vector<ItemDisplayData>& list, int startIndex = 0);
    
    // 연출 유틸리티
    void ShowLevelUpEvent(const std::string& name, int level);
    void ClearMainViewport();
    void ClearLog(); 
    void DrawImage(const std::string& imageAnsi);
    void DrawImageAtCenter(const std::string& imageAnsi);
    void DrawInputBox(const std::string& prompt = ""); 
    void SetCursorToInputArea(const std::string& prompt = ""); 

    void CenteredTypeLog(const std::string& text, int y, int delayMs = 100);
    void PlayBlueTransition();

    static void WaitKey(UIManager* ui); 

    std::string GetCurrentPrompt() const { return m_currentPrompt; }
};
