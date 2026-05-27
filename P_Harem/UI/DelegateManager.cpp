#include "DelegateManager.h"
#include "UIManager.h"
#include "../Battle/BattleSystem.h"

void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return;

    // TODO: 로직 팀원들의 델리게이트 구현에 따라 추가 바인딩
    /*
    battleSys->OnAttack = [ui](std::string name, int damage, std::string msg) {
        ui->PrintLog(name + "에게 " + std::to_string(damage) + msg);
    };
    */
}
