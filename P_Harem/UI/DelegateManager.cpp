#include "DelegateManager.h"
#include "UIManager.h"
#include "ImageManager.h"
#include "../Battle/BattleSystem.h"
#include "../GameManager/World.h"
#include <string>

/**
 * [BindAll] - 로직과 비주얼의 완벽한 융합
 */
void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return;

    // 1. 공격 신호 연결
    battleSys->OnAttack = [ui](std::string name) {
        ui->PrintLog("▶ " + name + "의 매력 발산! 분위기를 압도합니다.");
    };

    // 2. 피격 신호 연결 (실시간 UI 갱신 및 연출)
    battleSys->OnHit = [ui](std::string name, int damage, int multiplier) {
        std::string msg = "▶ " + name + "이(가) 큰 충격을 받았습니다! (체력 -" + std::to_string(damage) + ")";
        if (multiplier == 2) msg = "▶ [치명적 매력!] " + msg;
        else if (multiplier == 0) msg = "▶ [철벽 방어!] " + msg;
        ui->PrintLog(msg);
        
        C_World::GetInstance().SyncUI(); // 즉시 체력 바 업데이트
    };

    // 3. 패배/승리 신호 연결
    battleSys->OnDefeat = [ui](std::string name) {
        ui->PrintLog("▶ " + name + "이(가) 더 이상 버티지 못하고 물러납니다.");
        C_World::GetInstance().SyncUI();
        
        // [씬 전환] 전투 종료 후 다시 '방' 배경으로 조용히 전환합니다.
        ui->ClearMainViewport();
        ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Room", {}));
    };
}
