//#include "DelegateManager.h"
//#include "UIManager.h"
//#include "../Battle/BattleSystem.h" // 팀원 코드 인클루드

//void DelegateManager::BindAll(UIManager* ui, BattleSystem* battleSys) {
    // 1. 배틀 시스템에서 승리 신호가 오면 -> UI 매니저의 PrintLog를 실행해라!
    // battleSys->OnBattleWin = [ui](std::string heroineName, int money) {
    //     std::string logMsg = heroineName + "이(가) 승리하여 " + std::to_string(money) + "원을 얻었습니다!";
    //     ui->PrintLog(logMsg);
    // };

    // 2. 다른 팀원의 시스템(상점, 데이트 등)도 여기서 다 묶어줍니다.
//}

#include "DelegateManager.h"
#include "UIManager.h"
#include "../GameManager/World.h"
#include "../Battle/BattleSystem.h" // 팀원들의 시스템들...

void DelegateManager::Connect(C_World& world, UIManager& ui)
{
    // 1. 배틀 시스템 연결 (world를 통해 접근)
    //world.GetBattleSystem()->OnBattleWin = [&ui](std::string name, int money) {
    //    ui.PrintLog(name + " 승리! " + std::to_string(money) + "원 획득.");
    //};
        
    // 2. 플레이어 돈 변경 연결
    //world.GetPlayer()->OnMoneyChanged = [&ui](int currentMoney) {
    //    ui.UpdateMoney(currentMoney);
    //};
   
    // 3. 여기에 모든 팀원의 시스템을 하나씩 "배선"해 줍니다.
}