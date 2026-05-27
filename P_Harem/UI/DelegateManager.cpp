#include "DelegateManager.h"
#include "UIManager.h"
#include "../Battle/BattleSystem.h"

/**
 * [BindAll] 함수 - 전화국 교환원
 * 서로 존재를 모르는 로직(전투)과 화면(UI) 사이의 전선을 연결해 주는 아주 중요한 함수란다.
 * 이렇게 선을 연결해두면, 나중에 전투에서 승리했을 때 알아서 UI에 글자가 뜬단다.
 */
void DelegateManager::BindAll(UIManager* ui, C_BattleSystem* battleSys) {
    if (!ui || !battleSys) return; // 연결할 대상이 없으면 돌아갑니다.

    // [전투 신호 연결 예시]
    // 10년 뒤의 네가 팀원들과 협업할 때 이런 방식을 쓰면 아주 칭찬받을 거야.
    /*
    battleSys->OnAttack = [ui](std::string name) {
        // 누군가 공격하면 UI 로그창에 소식을 알립니다.
        ui->PrintLog(name + "의 치명적인 공격!");
    };
    */
    
    // 사용자님이 이 델리게이트를 통해 팀원들의 로직을 UI에 하나씩 꽂아주게 될 거란다.
}
