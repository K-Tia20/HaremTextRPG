#include "DelegateManager.h"
#include "UIManager.h"
#include "ImageManager.h"
#include "../Battle/BattleSystem.h"
#include "../GameManager/World.h"
#include <string>

/**
 * [BindAll] - 팀의 로직과 사용자님의 비주얼을 하나로 묶는 마법의 배선
 */
void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return;

    // 1. 공격 개시 신호
    battleSys->OnAttack = [ui](std::string name) {
        ui->PrintLog("\x1b[97m▶ [" + name + "]\x1b[0m이(가) 숨겨진 매력을 발산합니다!");
    };

    // 2. 피격 및 상성 결과 신호 (핵심 비주얼 FX)
    battleSys->OnHit = [ui](std::string name, int damage, int multiplier) {
        std::string effectPrefix = "";
        std::string damageColor = "\x1b[91m"; // 기본 대미지는 빨강

        if (multiplier == 2) {
            // [Critical FX] 유리한 상성일 때
            effectPrefix = "\x1b[93m[ CRITICAL HIT!! ] \x1b[0m";
            damageColor = "\x1b[93m"; // 황금색 강조
        } else if (multiplier == 0) {
            // [Guard FX] 불리한 상성일 때
            effectPrefix = "\x1b[90m[ GUARDED... ] \x1b[0m";
            damageColor = "\x1b[90m"; // 회색으로 약화 표시
        }

        ui->PrintLog(effectPrefix + "\x1b[97m" + name + "\x1b[0m에게 " + damageColor + std::to_string(damage) + " 대미지\x1b[0m를 입혔습니다!");
        
        // 실시간 스탯 바 업데이트 트리거
        C_World::GetInstance().SyncUI();
    };

    // 3. 전투 종료(승리/패배) 신호
    battleSys->OnDefeat = [ui](std::string name) {
        ui->PrintLog("\x1b[91m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
        ui->PrintLog("\x1b[97m  💀 [" + name + "]이(가) 매력에 취해 쓰러졌습니다!  \x1b[0m");
        ui->PrintLog("\x1b[91m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
        
        C_World::GetInstance().SyncUI(); // 최종 결과 반영
        
        // [씬 전환 연출] 전투가 끝나면 잠시 후 거점(내 방) 배경으로 부드럽게 복구
        ui->ClearMainViewport();
        ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Room", {}));
    };
}
