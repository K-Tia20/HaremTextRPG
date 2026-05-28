#include "DelegateManager.h"
#include "UIManager.h"
#include "ImageManager.h"
#include "ScriptManager.h"
#include "../Battle/BattleSystem.h"
#include "../GameManager/World.h"
#include <string>

// [팀 로직 연동] BattleSystem.cpp에 선언된 전역 레벨 시스템을 참조합니다.
extern C_LevelSystem LevelSystem;

/**
 * [BindAll] - 팀의 로직과 사용자님의 비주얼을 데이터 주도형으로 연결
 */
void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return;

    auto& script = C_ScriptManager::GetInstance();

    // 1. 공격 개시 신호
    battleSys->OnAttack = [ui, &script](std::string name) {
        ui->PrintLog(script.GetFormatStr("BATTLE_ATTACK_FX", {name}));
        // [SOUND] 여기에 공격 효과음을 추가하세요
    };

    // 2. 피격 및 상성 결과 신호
    battleSys->OnHit = [ui, &script](std::string name, int damage, int multiplier) {
        std::string effectPrefix = "";
        std::string damageColor = "\x1b[91m"; 

        if (multiplier == 2) {
            effectPrefix = script.Get("BATTLE_HIT_CRITICAL");
            damageColor = "\x1b[93m"; 
            // [SOUND] 여기에 크리티컬 효과음을 추가하세요
        } else if (multiplier == 0) {
            effectPrefix = script.Get("BATTLE_HIT_GUARD");
            damageColor = "\x1b[90m"; 
            // [SOUND] 여기에 방어 효과음을 추가하세요
        }

        ui->PrintLog(effectPrefix + script.GetFormatStr("BATTLE_HIT_FORMAT", {name, damageColor, std::to_string(damage)}));
        C_World::GetInstance().SyncUI(); 
    };

    // 3. 전투 종료 신호
    battleSys->OnDefeat = [ui, &script](std::string name) {
        ui->PrintLog(script.Get("UI_DIVIDER_RED"));
        ui->PrintLog("\x1b[97m" + script.GetFormatStr("BATTLE_DEFEAT_ALERT", {name}) + "\x1b[0m");
        ui->PrintLog(script.Get("UI_DIVIDER_RED"));
        
        // [SOUND] 여기에 패배/승리 효과음을 추가하세요
        
        C_World::GetInstance().SyncUI(); 
        
        // [사용자 요청] 전투가 끝나도 바로 배경을 방으로 바꾸지 않고 현재 배경(헌팅포차)을 유지합니다.
    };

    // 4. 아이템 사용 신호 연결
    battleSys->OnUseItem = [ui](std::string name, int effectType, int value) {
        std::string effectStr = "";
        if (effectType == 1) effectStr = "\x1b[91m공격력 증가\x1b[0m";
        else if (effectType == 2) effectStr = "\x1b[94m방어력 증가\x1b[0m";
        else if (effectType == 3) effectStr = "\x1b[92m체력 회복\x1b[0m";

        ui->PrintLog("\x1b[97m💊 [" + name + "]\x1b[0m이(가) 아이템을 사용하여 " + effectStr + " (+" + std::to_string(value) + ") 효과를 얻었습니다!");
    };

    // 5. [성장 이벤트] 레벨업 신호 연결
    LevelSystem.OnLevelUp = [ui](std::string name, int level) {
        // 뷰포트에 레벨업 팝업 출력
        ui->ShowLevelUpEvent(name, level);
        // [SOUND] 여기에 레벨업 축하 사운드를 추가하세요
        
        ui->PrintLog("\x1b[93m✨ [성장] " + name + "님의 등급이 Lv." + std::to_string(level) + "(으)로 상승했습니다! ✨\x1b[0m");
        ui->WaitEnterSilent();
        ui->ClearMainViewport();
        // 원래 배경 복구 (필요시)
        
    };
}
