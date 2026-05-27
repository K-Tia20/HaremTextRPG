
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

	ui->PrintLog("\x1b[95m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
	ui->PrintLog("\x1b[97m      🏙️  햇살 가득한 시내 광장        \x1b[0m");
	ui->PrintLog("\x1b[95m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
    ui->PrintLog("1. \x1b[91m🔥 핫플레이스 탐방 (헌팅 시도)\x1b[0m");
    ui->PrintLog("2. \x1b[92m📱 카카오톡 열기 (여친 목록)\x1b[0m");
    ui->PrintLog("3. \x1b[90m🚶 다른 지역으로 이동하기\x1b[0m");
    
	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1:
	{
		int randomIndex = rand() % 100;
		if (randomIndex < 70) Encounter();
		else {
            ui->PrintLog("\x1b[90m시스템: 한참을 서성였지만 허탕만 쳤습니다...\x1b[0m");
            UIManager::WaitKey(ui);
        }
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

	ui->PrintLog("\x1b[90m--- 어디로 갈까요? ---\x1b[0m");
    ui->PrintLog("1. \x1b[96m☕ 요거프레쏘 카페\x1b[0m");
    ui->PrintLog("2. \x1b[93m📋 오늘의 긴급 알바\x1b[0m");
    ui->PrintLog("0. 취소");

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
		CS = CityState::SelectMenu;
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
		ui->PrintLog("\x1b[90m시스템: 거리에는 더 이상 마주칠 사람이 없습니다...\x1b[0m");
        UIManager::WaitKey(ui);
		return;
	}

	int randomIndex = rand() % static_cast<int>(Girls.size());
	BattleGirl = Girls[randomIndex];

    // [Visual Upgrade] 그녀의 스타일색에 맞춘 화려한 등장
    std::string styleColor = "\x1b[37m";
    if (BattleGirl->GetStile() == C_Stile::HotGirl) styleColor = "\x1b[31m";
    else if (BattleGirl->GetStile() == C_Stile::IceGirl) styleColor = "\x1b[36m";
    else if (BattleGirl->GetStile() == C_Stile::GrassGirl) styleColor = "\x1b[32m";

    ui->PrintLog("\x1b[95m[ ! ] 운명적인 만남!\x1b[0m");
    ui->PrintLog(styleColor + "★ 저 멀리서 [" + BattleGirl->GetName() + "]이(가) 나타났습니다! ★\x1b[0m");
	
	auto FightGirl = Player->SetFightGirl();
    ui->ClearLog(); 

	if (FightGirl == nullptr)
	{
		ui->PrintLog("시스템: 오늘은 이만 물러나기로 했습니다.");
        UIManager::WaitKey(ui);
		return;
	}

    ui->PrintLog("\x1b[93m⚔️ 매력 배틀 START! ⚔️\x1b[0m");
    ui->PrintLog("[\x1b[92m" + FightGirl->GetName() + "\x1b[0m] VS [\x1b[91m" + BattleGirl->GetName() + "\x1b[0m]");
    ui->PrintLog("\x1b[90m------------------------------------\x1b[0m");

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
    auto ui = World->GetUI();
	int randomIndex = rand() % 100;
	if (randomIndex < 40) // 확률 소폭 상향
	{
		BattleGirl->SetMaxHp(200);
		BattleGirl->SetAttack(30);
		Player->AddGirlFrends(BattleGirl);
		Girls.erase(remove(Girls.begin(), Girls.end(), BattleGirl), Girls.end());
        ui->PrintLog("\x1b[93m✨ 대성공! [" + BattleGirl->GetName() + "]의 연락처를 얻었습니다! ✨\x1b[0m");
	} else {
        ui->PrintLog("\x1b[90m[" + BattleGirl->GetName() + "]은(는) 부끄러워하며 도망갔습니다...\x1b[0m");
    }
}

void C_City::ViewYeuchin()
{
    auto ui = World->GetUI();
    if (!ui) return;

    ui->PrintLog("\x1b[92m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
    ui->PrintLog("\x1b[97m      📱 등록된 여자친구 리스트       \x1b[0m");
    ui->PrintLog("\x1b[92m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
	auto friends = Player->GetGirlFrends();
    if (friends.empty()) ui->PrintLog("\x1b[90m(아직 텅 비어 있습니다...)\x1b[0m");
    else {
        for (const auto& f : friends) {
            ui->PrintLog("💖 \x1b[97m" + f->GetName() + "\x1b[0m (HP: " + to_string(f->GetCurrentHp()) + ")");
        }
    }
    ui->PrintLog("\x1b[90m------------------------------------\x1b[0m");
    UIManager::WaitKey(ui);
}
