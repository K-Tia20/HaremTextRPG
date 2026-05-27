#include "DelegateManager.h"
#include "UIManager.h"
#include "../Battle/BattleSystem.h"
#include "../GameManager/World.h"
#include <string>

/**
 * [BindAll] - 배틀 시스템과 UI를 연결하는 심장부
 */
void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return;

    // 1. 공격 신호 연결
    battleSys->OnAttack = [ui](std::string name) {
        ui->PrintLog("▶ " + name + "의 매력 발산! 분위기를 압도합니다.");
    };

    // 2. 피격 신호 연결 - [중요] 실시간 UI 동기화 추가
    battleSys->OnHit = [ui](std::string name, int damage, int multiplier) {
        std::string msg = "▶ " + name + "이(가) 큰 충격을 받았습니다! (체력 -" + std::to_string(damage) + ")";
        if (multiplier == 2) msg = "▶ [치명적 매력!] " + msg;
        else if (multiplier == 0) msg = "▶ [철벽 방어!] " + msg;
        ui->PrintLog(msg);

        // [실시간 반영] 데미지를 입는 즉시 월드의 SyncUI를 호출하여 게이지를 깎습니다.
        C_World::GetInstance().SyncUI();
    };

    // 3. 패배/승리 신호 연결
    battleSys->OnDefeat = [ui](std::string name) {
        ui->PrintLog("▶ " + name + "이(가) 더 이상 버티지 못하고 물러납니다.");
        C_World::GetInstance().SyncUI(); // 최종 결과 반영
    };
}
