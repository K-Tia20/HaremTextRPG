#pragma once
#include <string>
#include <vector>
#include "UILayoutDefs.h"

class UIManager {
private:
    int GetVisualLength(const std::string& str);
    void DrawSolidBox(int startX, int startY, int width, int height, std::string title);
    void DrawSmartphoneFrame(int startX, int startY, int width, int height);
    void gotoxy(int x, int y);

public:
    void Init(); 
    void RenderMainUI(); 

    // 데이터 업데이트 API
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

/*
// --- main.cpp ---
#include "UIManager.h"
#include "DelegateManager.h"
#include "BattleSystem.h" // 예시
#include <iostream>

int main() {
    // 1. 객체 생성
    UIManager ui;
    DelegateManager delegateMgr;
    BattleSystem battleSys; 

    // 2. UI 초기화 및 렌더링
    ui.Init();
    ui.RenderMainUI();

    // 3. 델리게이트 매니저를 통해 UI와 시스템의 통신선 연결
    delegateMgr.BindAll(&ui, &battleSys);

    // 4. 게임 메인 루프 시작...
    // 유저 입력 등에 따라 battleSys.StartBattle(); 이 실행되면
    // 자동으로 UI에 텍스트가 뜹니다!
    
    std::cin.get();
    return 0;
}
 */