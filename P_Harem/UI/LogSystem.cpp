#include "LogSystem.h"
#include "UIManager.h"
#include "ScriptManager.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Creature/Creature.h" // 히로인 정보를 읽기 위해 상세 정의서를 가져옵니다.
#include <iostream>

/**
 * [AddCalendarLog] 함수
 * 게임 속에서 일어나는 모든 소중한 일들을 일기장(m_calendarLogs)에 적어두는 함수야.
 */
void C_LogSystem::AddCalendarLog(const std::string& logText) {
    m_calendarLogs.push_back(logText); // 새로운 추억을 목록에 추가!
}

/**
 * [ShowSmartphoneMenu] 함수
 * 우리가 만든 스마트폰 UI를 통해 캘린더나 연락처를 확인하는 로직을 관리한단다.
 */
void C_LogSystem::ShowSmartphoneMenu() {
    UIManager* ui = C_World::GetInstance().GetUI(); // 화가를 불러와요.
    auto& script = C_ScriptManager::GetInstance(); // 대본을 불러와요.

    // 1. 작가님이 정해주신 메뉴 안내 문구를 출력합니다.
    ui->PrintLog(script.Get("PHONE_MENU_MAIN"));
    
    // 2. 사용자의 선택을 기다립니다 (1. 캘린더, 2. 연락처...)
    int choice = C_Player::Input<int>();
    
    switch (choice) {
        case 1:
            ui->PrintLog(script.Get("PHONE_CALENDAR_TITLE")); // "--- 나의 캘린더 ---"
            if (m_calendarLogs.empty()) {
                ui->PrintLog(script.Get("PHONE_CALENDAR_EMPTY")); // "기록이 없습니다."
            } else {
                for (const auto& log : m_calendarLogs) {
                    ui->PrintLog("- " + log); // 저장된 일기들을 하나씩 읽어줘요.
                }
            }
            break;
        case 2:
            ShowContactList(); // 연락처를 보여주는 함수로 이동!
            break;
        default:
            ui->PrintLog(script.Get("PHONE_BACK_MAIN")); // "메인으로 돌아갑니다."
            break;
    }
}

/**
 * [ShowContactList] 함수
 * 내 스마트폰에 저장된 그녀들의 이름과 상태를 화면에 예쁘게 리스트로 뽑아준단다.
 */
void C_LogSystem::ShowContactList() {
    UIManager* ui = C_World::GetInstance().GetUI();
    
    ui->PrintLog(C_ScriptManager::GetInstance().Get("PHONE_CONTACT_SYNC")); // "동기화 중..."
    
    // 플레이어가 실제로 소유한 히로인들의 정보를 UI용 데이터로 바꿉니다.
    std::vector<UIManager::HeroineDisplayData> displayList;
    const auto& friends = C_World::GetInstance().GetPlayer()->GetGirlFrends();
    
    for (const auto& f : friends) {
        displayList.push_back({
            f->GetName(), 
            f->GetCurrentHp(), 
            f->GetMaxHp(), 
            "Lv." + std::to_string(f->GetLevel())
        });
    }
    
    // 3. 사용자님이 정성껏 만든 스마트폰 UI 화면에 목록을 띄워줍니다.
    ui->UpdateHeroineList(displayList);
}

void C_LogSystem::ShowInventory() {
    UIManager* ui = C_World::GetInstance().GetUI();
    ui->PrintLog(C_ScriptManager::GetInstance().Get("PHONE_INV_OPEN")); // "가방을 엽니다."
    // 나중에 실제 아이템 목록을 보여주는 로직이 여기 들어갈 거야.
    ui->PrintLog("[ 된장찌개 ], [ 삼겹살 ] ... (개발 중)");
}
