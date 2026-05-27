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
    auto& script = C_ScriptManager::GetInstance();

    ui->PrintLog(script.Get("PHONE_MENU_MAIN"));
    
    int choice = C_Player::Input<int>();
    
    switch (choice) {
        case 1:
            ui->PrintLog(script.Get("PHONE_CALENDAR_TITLE"));
            if (m_calendarLogs.empty()) {
                ui->PrintLog(script.Get("PHONE_CALENDAR_EMPTY"));
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
            ui->PrintLog(script.Get("PHONE_BACK_MAIN"));
            break;
    }
}

void C_LogSystem::ShowContactList() {
    UIManager* ui = C_World::GetInstance().GetUI();
    auto& script = C_ScriptManager::GetInstance();
    
    ui->PrintLog(script.Get("PHONE_CONTACT_SYNC"));
    
    std::vector<UIManager::HeroineDisplayData> displayList;
    
    // TODO: 로직 연동 시 GetFormatStr 등을 활용해 가변 텍스트 구성 가능
    ui->UpdateHeroineList(displayList);
}

void C_LogSystem::ShowInventory() {
    UIManager* ui = C_World::GetInstance().GetUI();
    auto& script = C_ScriptManager::GetInstance();

    ui->PrintLog(script.Get("PHONE_INV_OPEN"));
    ui->PrintLog("[ 된장찌개 ], [ 삼겹살 ] ... (개발 중)");
}
