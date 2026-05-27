
#include "../Framework/Framework.h"
#include "../Battle/BattleSystem.h"
#include "../Areas/Area.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "../Framework/CreatureInclude.h"
#include "City.h"

using namespace std;

C_City::C_City(C_World* world)
{
	C_Area::SetWorld(world);
	Player = World->GetPlayer();
	Battle = World->GetBattle();

	Girls.push_back(make_shared<And>());
	Girls.push_back(make_shared<Chashrin>());
	Girls.push_back(make_shared<Codelia>());
	Girls.push_back(make_shared<Lina>());
	Girls.push_back(make_shared<Marry>());
	Girls.push_back(make_shared<Oplier>());
	Girls.push_back(make_shared<Zhad>());
	Girls.push_back(make_shared<C_Creature>("지나가는 행인", C_Stile::NormalGirl, 200, 30));
}

void C_City::SelectMenu()
{
    auto ui = World->GetUI();
    if (!ui) return;

	ui->PrintLog("--- 시내 광장 ---");
    ui->PrintLog("1. 헌팅 시도 (전투)");
    ui->PrintLog("2. 여자친구 목록 보기");
    ui->PrintLog("3. 다른 지역으로 이동");
    
	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1:
	{
		int randomIndex = rand() % 100;
		if (randomIndex < 70) Encounter();
		else ui->PrintLog("오늘은 허탕 쳤습니다. 조용히 집으로 돌아갑니다.");
	}
		break;
	case 2:
		ViewYeuchin();
		break;
	case 3:
		CS = CityState::MoveArea;
		break;
	default:
		ui->PrintLog("시스템: 잘못된 입력입니다.");
		break;
	}
}

void C_City::MoveArea()
{
    auto ui = World->GetUI();
    if (!ui) return;

	ui->PrintLog("--- 지역 이동 ---");
    ui->PrintLog("1. 상점 (요거프레쏘)");
    ui->PrintLog("2. 알바하러 가기");

	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1:
		CS = CityState::SelectMenu;
		World->GotoShop();
		break;
	case 2:
		CS = CityState::SelectMenu;
		World->GotoAlba();
		break;
	default:
		ui->PrintLog("시스템: 다시 골라주세요...");
		break;
	}
}

void C_City::Update()
{
	switch (CS)
	{
	case CityState::SelectMenu: SelectMenu(); break;
	case CityState::MoveArea: MoveArea(); break;
	}
}

void C_City::Encounter()
{
    auto ui = World->GetUI();
    if (!ui) return;

	if (Girls.empty())
	{
		ui->PrintLog("시스템: 더 이상 만날 사람이 없습니다.");
		return;
	}

	int randomIndex = rand() % static_cast<int>(Girls.size());
	BattleGirl = Girls[randomIndex];

    ui->PrintLog("운명적인 만남! [" + BattleGirl->GetName() + "]을(를) 만났습니다.");
	
	auto FightGirl = Player->SetFightGirl();
    ui->ClearLog(); 

	if (FightGirl == nullptr)
	{
		ui->PrintLog("시스템: 전투를 취소했습니다.");
		return;
	}

    ui->PrintLog("=== 배틀 시작: [" + FightGirl->GetName() + "] VS [" + BattleGirl->GetName() + "] ===");
	Battle->Battle(FightGirl, BattleGirl);

    UIManager::WaitKey(ui);

	// 여친 획득
	if (BattleGirl->IsDefeated()) Gatcha();

	// 여친 방생
	if (FightGirl->IsDefeated())
	{
		Player->RemoveGirlFriend(FightGirl);
	}
}

void C_City::Gatcha()
{
	int randomIndex = rand() % 100;
	if (randomIndex < 30)
	{
		BattleGirl->SetMaxHp(200);
		BattleGirl->SetAttack(30);
		Player->AddGirlFrends(BattleGirl);
		Girls.erase(remove(Girls.begin(), Girls.end(), BattleGirl), Girls.end());
	}
}

void C_City::ViewYeuchin()
{
    auto ui = World->GetUI();
    if (!ui) return;

    ui->PrintLog("--- 내 여자친구들 ---");
	auto friends = Player->GetGirlFrends();
    if (friends.empty()) ui->PrintLog("(아직 아무도 없습니다...)");
    else {
        for (const auto& f : friends) {
            ui->PrintLog("- " + f->GetName() + " (HP: " + to_string(f->GetCurrentHp()) + ")");
        }
    }
}
