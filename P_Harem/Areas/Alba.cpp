#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "Alba.h"
#include <windows.h>

using namespace std;

C_Alba::C_Alba(C_World* world)
{
	World = world;
	Player = World->GetPlayer();
}

void C_Alba::SelectMenu()
{
    auto ui = World->GetUI();
    if (!ui) return;

    // [Visual Upgrade] 앱 느낌의 메뉴 구성
	ui->PrintLog("\x1b[93m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
	ui->PrintLog("\x1b[97m      📢 오늘의 긴급 알바 모집      \x1b[0m");
	ui->PrintLog("\x1b[93m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
	ui->PrintLog("1. \x1b[96m📦 쿠팡 물류 야간 상하차\x1b[0m");
	ui->PrintLog("2. \x1b[92m🥕 당근마켓 매너온도 빌런 잡기\x1b[0m");
	ui->PrintLog("3. \x1b[95m👰 결혼식 가짜 하객 프로 대행\x1b[0m");
	ui->PrintLog("4. \x1b[93m🛵 배민커넥트 지옥의 도보 배달\x1b[0m");
	ui->PrintLog("\x1b[90m------------------------------------\x1b[0m");
	ui->PrintLog("5. 🏠 돌아가기");

	int choice = Player->InputInt();
    ui->ClearLog(); 

    string jobName = "";
    int pay = 100000; // 팀의 기본 보상 유지

	switch (choice)
	{
	case 1: jobName = "쿠팡 상하차"; break;
	case 2: jobName = "빌런 검거"; break;
	case 3: jobName = "하객 대행"; break;
	case 4: jobName = "도보 배달"; break;
	default: AS = AlbaState::MoveArea; return;
	}

    // [Visual Upgrade] 알바 진행 애니메이션
    ui->PrintLog("\x1b[97m[" + jobName + "] \x1b[0m작업을 시작합니다...");
    for (int i = 1; i <= 5; ++i) {
        string gage = "[";
        for (int j = 0; j < 5; ++j) gage += (j < i) ? "\x1b[93m■\x1b[0m" : "\x1b[90m□\x1b[0m";
        gage += "] \x1b[90m열심히 하는 중...\x1b[0m";
        ui->PrintLog(gage, true); // Overlap 출력
        Sleep(400);
    }

    ui->ClearLog();
	ui->PrintLog("\x1b[92m✔ " + jobName + " 완료!\x1b[0m");
	ui->PrintLog("\x1b[93m💰 일당 " + to_string(pay) + " 원이 입금되었습니다!\x1b[0m");
    Player->AddMoney(pay);
    
    UIManager::WaitKey(ui);
}

void C_Alba::MoveArea()
{
	AS = AlbaState::SelectMenu;
	World->GotoCity();
}

void C_Alba::Update()
{
	switch (AS)
	{
	case AlbaState::SelectMenu:
		SelectMenu();
		break;
	case AlbaState::MoveArea:
		MoveArea();
		break;
	}
}
