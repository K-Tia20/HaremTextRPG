#include "LogSystem.h"
#include "UIManager.h"
#include "ScriptManager.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Creature/Creature.h" 
#include "../Inventory/Inventory.h"
#include "../Objects/Items/Item.h"
#include <iostream>

void C_LogSystem::AddCalendarLog(const std::string& logText) {
    m_calendarLogs.push_back(logText); 
}

void C_LogSystem::ShowSmartphoneMenu() {
    UIManager* ui = C_World::GetInstance().GetUI(); 
    auto& script = C_ScriptManager::GetInstance(); 
    auto player = C_World::GetInstance().GetPlayer();
    if (!player || !ui) return;

    ui->PrintLog(script.Get("PHONE_MENU_MAIN"));
    int choice = player->InputInt();
    
    switch (choice) {
        case 1:
            ui->PrintLog("\x1b[96m--- 📅 캘린더 기록 ---\x1b[0m"); 
            if (m_calendarLogs.empty()) {
                ui->PrintLog("\x1b[90m(기록된 사건이 없습니다.)\x1b[0m"); 
            } else {
                for (const auto& log : m_calendarLogs) {
                    ui->PrintLog("• " + log); 
                }
            }
            UIManager::WaitKey(ui);
            break;
        case 2:
            ShowContactList(); 
            break;
        default:
            break;
    }
}

void C_LogSystem::ShowContactList() {
    UIManager* ui = C_World::GetInstance().GetUI();
    if (!ui) return;
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
    UIManager::WaitKey(ui);
}

void C_LogSystem::ShowInventory() {
    UIManager* ui = C_World::GetInstance().GetUI();
    auto player = C_World::GetInstance().GetPlayer();
    if (!ui || !player) return;

    auto inv = player->GetInventory();
    if (!inv) return;

    ui->PrintLog("\x1b[95m--- 🎒 가방 열기 ---\x1b[0m"); 
    
    std::vector<UIManager::ItemDisplayData> displayList;
    for (int i = 0; i < inv->GetSize(); ++i) {
        auto itemObj = inv->GetItem(i);
        if (!itemObj) continue;

        const auto& data = itemObj->GetItem();
        std::string typeLabel = "기타";
        if (data.Type == ItemType::Heal) typeLabel = "🍱 회복";
        else if (data.Type == ItemType::Power) typeLabel = "🍗 강화";
        else if (data.Type == ItemType::Defence) typeLabel = "🛡️ 방어";

        displayList.push_back({
            data.Name,
            data.Quantity,
            data.Value,
            typeLabel
        });
    }
    
    ui->UpdateInventoryList(displayList);
    UIManager::WaitKey(ui);
}
