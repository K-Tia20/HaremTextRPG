#include "LogSystem.h"
#include "UIManager.h"
#include "ScriptManager.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include <iostream>

void C_LogSystem::AddCalendarLog(const std::string& logText) {
    m_calendarLogs.push_back(logText);
}

void C_LogSystem::ShowSmartphoneMenu() {
    UIManager* ui = C_World::GetInstance().GetUI();
    ui->PrintLog("스마트폰을 확인합니다. (1. 캘린더, 2. 연락처, 0. 뒤로가기)");
    
    // 팀원들의 입력 방식(Player->Input)을 그대로 사용하여 통일감을 유지합니다.
    int choice = C_Player::Input<int>();
    
    switch (choice) {
        case 1:
            ui->PrintLog("--- [ 나의 캘린더 ] ---");
            if (m_calendarLogs.empty()) {
                ui->PrintLog("아직 기록된 일정이 없습니다.");
            } else {
                for (const auto& log : m_calendarLogs) {
                    ui->PrintLog("- " + log);
                }
            }
            break;
        case 2:
            ShowContactList();
            break;
        default:
            ui->PrintLog("메인 메뉴로 돌아갑니다.");
            break;
    }
}

void C_LogSystem::ShowContactList() {
    UIManager* ui = C_World::GetInstance().GetUI();
    C_Player* player = C_World::GetInstance().GetPlayer();
    
    ui->PrintLog("연락처 목록을 동기화합니다...");
    
    // 플레이어가 가진 히로인 목록을 UI용 구조체로 변환하여 뿌려줍니다.
    // 이는 사용자님이 만드신 UpdateHeroineList 함수를 100% 활용하는 방식입니다.
    std::vector<UIManager::HeroineDisplayData> displayList;
    
    // TODO: 플레이어 클래스의 GetGirlFrends()와 연동 필요
    /*
    auto& friends = player->GetGirlFrends();
    for (auto& f : friends) {
        displayList.push_back({f->GetName(), f->GetCurrentHp(), f->GetMaxHp(), "Lv." + std::to_string(f->GetLevel())});
    }
    */
    
    ui->UpdateHeroineList(displayList);
}

void C_LogSystem::ShowInventory() {
    UIManager* ui = C_World::GetInstance().GetUI();
    ui->PrintLog("가방을 열어 아이템을 확인합니다.");
    
    // 기획서대로 콤마(,)로 구분하여 출력하거나 UI에 예쁘게 정렬합니다.
    ui->PrintLog("[ 된장찌개 ], [ 삼겹살 ] ... (개발 중)");
}
