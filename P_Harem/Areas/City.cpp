
#include "../Framework/Framework.h"
#include "../Battle/BattleSystem.h"
#include "../Areas/Area.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "../UI/ImageManager.h"
#include "../UI/ScriptManager.h"
#include "../Framework/CreatureInclude.h"
#include "../UI/LogSystem.h"
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
	Girls.push_back(make_shared<C_Creature>("평범녀", C_Stile::NormalGirl, 200, 30));
}

void C_City::SelectMenu()
{
    auto ui = World->GetUI();
    auto& script = C_ScriptManager::GetInstance();
    if (!ui) return;

	ui->PrintLog(script.Get("UI_DIVIDER_PURPLE"));
	ui->PrintLog(script.Get("CITY_HEADER"));
	ui->PrintLog(script.Get("UI_DIVIDER_PURPLE"));
	ui->PrintLog(script.Get("CITY_MENU_1"));
	ui->PrintLog(script.Get("CITY_MENU_2"));
	ui->PrintLog(script.Get("CITY_MENU_3"));
    
	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1:
		{
			// [Visual Upgrade] 헌팅포차 진입 배경 출력
			ui->ClearMainViewport();
			ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
			
			if (Player->GetHighlevel() >= 10)
			{
				// TODO: 보스전 돌입 전 대사
				ui->PrintLog("\x1b[90m이젠 준비된거 같다...주체할 수 없는 내 마음을 전하러갈까?\x1b[0m");
				ui->PrintLog("\x1b[90m1. 가자\x1b[0m");
				ui->PrintLog("\x1b[90m0. 안가\x1b[0m");
        	
				if (Player->InputInt() == 1) {
					Battle->BossBattle(Player->SetFightGirl());
				} else {
					ui->ClearLog();
				}
			}
			else // 레벨 10 미만 일반 헌팅 조우
			{
				int randomIndex = rand() % 100;
				if (randomIndex < 70) Encounter();
				else {
					ui->PrintLog("\x1b[90m시스템: 한참을 서성였지만 허탕만 쳤습니다...\x1b[0m");
					UIManager::WaitKey(ui);
				}
			}
        
			// 헌팅(전투 성공, 실패, 도망, 허탕, 보스전)이 끝나면 무조건 귀가하며 하루가 지남
			World->SetHubBG("BG_Room");
			World->AdvanceDay();
			CS = CityState::Exit;
		}
		break;
	case 2:
		// ViewYeuchin(); 대신 좌측 스마트폰 스크롤 UI 호출!
		C_LogSystem::GetInstance().ShowContactList();
		break;
	case 0:
        World->SetHubBG("BG_City");
		CS = CityState::Exit;
		break;
	default:
		ui->PrintLog("시스템: 잘못된 입력입니다.");
        UIManager::WaitKey(ui);
		break;
	}
}

void C_City::Update()
{
    bool isLooping = true;
	while (isLooping)
	{
		switch (CS)
		{
		case CityState::SelectMenu: 
            SelectMenu(); 
            break;
		case CityState::Exit: 
            isLooping = false;
            break;
		}
	}
}

void C_City::Encounter()
{
    auto ui = World->GetUI();
	auto ui = World->GetUI();
	auto& script = C_ScriptManager::GetInstance();
	if (!ui) return;

	if (Girls.empty())
	{
		ui->PrintLog("\x1b[90m시스템: 거리에는 더 이상 마주칠 사람이 없습니다...\x1b[0m");
		UIManager::WaitKey(ui);
		return;
	}

	int randomIndex = rand() % static_cast<int>(Girls.size());
	BattleGirl = Girls[randomIndex];

    std::string styleColor = "\x1b[37m";
    if (BattleGirl->GetStile() == C_Stile::HotGirl) styleColor = "\x1b[31m";
	std::string styleColor = "\x1b[37m";
	if (BattleGirl->GetStile() == C_Stile::HotGirl) styleColor = "\x1b[31m";
	else if (BattleGirl->GetStile() == C_Stile::IceGirl) styleColor = "\x1b[36m";
	else if (BattleGirl->GetStile() == C_Stile::GrassGirl) styleColor = "\x1b[32m";

	// [추가된 연출] 내 여친을 고르기 전에, 헌팅포차 배경과 마주친 적을 정중앙에 띄우기
	ui->ClearMainViewport();
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {
		{BattleGirl->GetImageKey(), 50, 0, false}
	}));

	ui->PrintLog(script.Get("CITY_ENCOUNTER_ALERT"));
	ui->PrintLog(script.GetFormatStr("CITY_HEROINE_APPEAR", {styleColor, BattleGirl->GetName()}));
	
	// 그 후에 내 여친을 선택합니다.
	auto FightGirl = Player->SetFightGirl();
	ui->ClearLog();

	if (FightGirl == nullptr)
	{
		ui->PrintLog("시스템: 오늘은 이만 물러나기로 했습니다.");
        UIManager::WaitKey(ui);
		return;
	}

    // [Visual Upgrade] 헌팅포차 배경 + 내 여친(좌측/반전) + 적 여친(우측) 레이어링
    ui->ClearMainViewport();
    ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {
        {FightGirl->GetImageKey(), 5, 0, true}, // 플레이어 여친: 좌측 위치, 좌우 반전 적용
        {BattleGirl->GetImageKey(), 97, 0, false} // 상대 여친: 우측 위치로 더 밀어냄
    }));

    std::string myStyleColor = "\x1b[37m";
    if (FightGirl->GetStile() == C_Stile::HotGirl) myStyleColor = "\x1b[31m";
    else if (FightGirl->GetStile() == C_Stile::IceGirl) myStyleColor = "\x1b[36m";
    else if (FightGirl->GetStile() == C_Stile::GrassGirl) myStyleColor = "\x1b[32m";

    ui->PrintLog(script.Get("CITY_BATTLE_TITLE"));
    ui->PrintLog(script.GetFormatStr("CITY_BATTLE_VS", {myStyleColor + FightGirl->GetName(), styleColor + BattleGirl->GetName()}));
    ui->PrintLog(script.Get("UI_LINE_THIN"));

	Battle->Battle(FightGirl, BattleGirl);

    UIManager::WaitKey(ui);

	if (BattleGirl->IsDefeated()) Gatcha();
	if (FightGirl->IsDefeated()) { Player->RemoveGirlFriend(FightGirl); }
}

void C_City::Gatcha()
{
	auto ui = World->GetUI();
	auto& script = C_ScriptManager::GetInstance();
	
	int randomIndex = rand() % 100;
    
	ui->ClearMainViewport(); // 1. 일단 뷰포트를 깨끗하게 비웁니다.
    
	if (randomIndex < 40) 
	{
		if (BattleGirl->GetName() == "평범녀")
		{
			ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {})); // 배경만 출력
			ui->PrintLog(script.GetFormatStr("CITY_CONTACT_FAIL", {BattleGirl->GetName()}));
			UIManager::WaitKey(ui); // 대기
			return ;
		}
		//레벨을 1로 리셋
		BattleGirl->SetLevel(1);
		BattleGirl->SetMaxHp(200);
		BattleGirl->SetCurrentHp(200);
		BattleGirl->SetAttack(30);
        
		Player->AddGirlFrends(BattleGirl);
		Girls.erase(remove(Girls.begin(), Girls.end(), BattleGirl), Girls.end());
        
		// 2. 획득 성공! 헌팅포차 배경 + 해당 히로인을 정중앙(50%)에 출력
		ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {{BattleGirl->GetImageKey(), 50, 0, false}}));
		ui->PrintLog(script.GetFormatStr("CITY_CONTACT_SUCCESS", {BattleGirl->GetName()}));
	} else {
		// 3. 획득 실패! 헌팅포차 배경만 쓸쓸하게 출력
		ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
		ui->PrintLog(script.GetFormatStr("CITY_CONTACT_FAIL", {BattleGirl->GetName()}));
	}
    
	// 4. 결과를 천천히 감상할 수 있도록 엔터 대기
	UIManager::WaitKey(ui); 
}

void C_City::ViewYeuchin()
{
    auto ui = World->GetUI();
    auto& script = C_ScriptManager::GetInstance();
    if (!ui) return;

    ui->PrintLog(script.Get("UI_DIVIDER_PURPLE"));
    ui->PrintLog("\x1b[97m      📱 등록된 여자친구 리스트       \x1b[0m");
    ui->PrintLog(script.Get("UI_DIVIDER_PURPLE"));
	auto friends = Player->GetGirlFrends();
    if (friends.empty()) ui->PrintLog("\x1b[90m(아직 텅 비어 있습니다...)\x1b[0m");
    else {
        for (const auto& f : friends) {
            ui->PrintLog("💖 \x1b[97m" + f->GetName() + "\x1b[0m (HP: " + to_string(f->GetCurrentHp()) + ")");
        }
    }
    ui->PrintLog(script.Get("UI_LINE_THIN"));
    UIManager::WaitKey(ui);
}
