#include "LogSystem.h"
#include "UIManager.h"
#include "ScriptManager.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Creature/Creature.h" 
#include <iostream>

void C_LogSystem::AddCalendarLog(const std::string& logText) {
    m_calendarLogs.push_back(logText); 
}

void C_LogSystem::ShowSmartphoneMenu() {
    UIManager* ui = C_World::GetInstance().GetUI(); 
    auto& script = C_ScriptManager::GetInstance(); 
    auto player = C_World::GetInstance().GetPlayer();
    if (!player) return;

    ui->PrintLog(script.Get("PHONE_MENU_MAIN"));
    int choice = player->InputInt();
    
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
    ui->PrintLog(C_ScriptManager::GetInstance().Get("PHONE_CONTACT_SYNC")); 
    
    std::vector<UIManager::HeroineDisplayData> displayList;
    auto player = C_World::GetInstance().GetPlayer();
    if (!player) return;

    const auto& friends = player->GetGirlFrends();
    for (const auto& f : friends) {
        std::string sStyle = "평범녀";
        if (f->GetStile() == C_Stile::HotGirl) sStyle = "핫걸";
        else if (f->GetStile() == C_Stile::IceGirl) sStyle = "냉미녀";
        else if (f->GetStile() == C_Stile::GrassGirl) sStyle = "초식녀";

        displayList.push_back({
            f->GetName(), 
            f->GetLevel(), 
            f->GetCurrentHp(), 
            f->GetMaxHp(), 
            f->GetAttack(), 
            f->GetAffinity(), 
            sStyle
        });
    }
    ui->UpdateHeroineList(displayList);
}

void C_LogSystem::ShowInventory() {
    UIManager* ui = C_World::GetInstance().GetUI();
    ui->PrintLog(C_ScriptManager::GetInstance().Get("PHONE_INV_OPEN")); 
    ui->PrintLog("[ 된장찌개 ], [ 삼겹살 ] ... (개발 중)");
}
