
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "Alba.h"

using namespace std;

C_Alba::C_Alba(C_World* world)
{
	World = world;
	Player = World->GetPlayer();
}

void C_Alba::SelectMenu()
{
    auto ui = World->GetUI();
	ui->PrintLog("--- 알바 구직 ---");
	ui->PrintLog("1. 쿠팡 물류센터 야간 상하차");
	ui->PrintLog("2. 당근마켓 빌런 잡기");
	ui->PrintLog("3. 결혼식 가짜 하객 대행");
	ui->PrintLog("4. 배민커넥트 도보 배달");
	ui->PrintLog("5. 돌아가기");

	int choice = Player->InputInt();
    ui->ClearLog(); // 선택 후 로그 청소

	switch (choice)
	{
	case 1:
		ui->PrintLog("시스템: 쿠팡 상하차 완료! 10골드를 벌었습니다.");
		Player->AddMoney(10);
		break;
	case 2:
		ui->PrintLog("시스템: 빌런 검거 완료! 10골드를 벌었습니다.");
		Player->AddMoney(10);
		break;
	case 3:
		ui->PrintLog("시스템: 하객 대행 완료! 10골드를 벌었습니다.");
		Player->AddMoney(10);
		break;
	case 4:
		ui->PrintLog("시스템: 배달 완료! 10골드를 벌었습니다.");
		Player->AddMoney(10);
		break;
	default:
		AS = AlbaState::MoveArea;
		break;
	}
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
