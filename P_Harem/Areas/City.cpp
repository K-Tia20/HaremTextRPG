
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

				int choice = Player->InputInt();
        	
				if (choice == 1) {
					Battle->BossBattle(Player->SetFightGirl());
					return;
				}
				else if(choice == 0)
				{
					ui->ClearLog();
				}
				return;
			}

			int randomIndex = rand() % 100;
			if (randomIndex < 70) Encounter();
			else {
				ui->PrintLog("\x1b[90m⚙️시스템: 한참을 서성였지만 허탕만 쳤습니다...\x1b[0m");
				UIManager::WaitKey(ui);
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
		ui->PrintLog("\x1b[90m⚙️시스템: 잘못된 입력입니다.");
        UIManager::WaitKey(ui);
		break;
	}
}

void C_City::Update()
{
    bool isLooping = true;

	
	if (Player->GetGirlFrends().empty())
	{
		World->SoloEnd();
		CS = CityState::Exit;
	}
	
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
	auto& script = C_ScriptManager::GetInstance();
	if (!ui) return;

	if (Girls.empty())
	{
		ui->PrintLog("\x1b[90m⚙️ 시스템: 거리에는 더 이상 마주칠 사람이 없습니다...\x1b[0m");
		UIManager::WaitKey(ui);
		return;
	}

	int randomIndex = rand() % static_cast<int>(Girls.size());
	BattleGirl = Girls[randomIndex];
	
	// [추가된 연출] 헌팅포차 배경과 마주친 적 정중앙 띄우기
	ui->ClearMainViewport();
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {
		{BattleGirl->GetImageKey(), 50, 0, false}
	}));

	ui->PrintLog(script.Get("CITY_ENCOUNTER_ALERT"));
	// 🚨 수정완료: GetColoredName() 하나만 넘깁니다!
	ui->PrintLog(script.GetFormatStr("CITY_HEROINE_APPEAR", {BattleGirl->GetColoredName()}));
	
	auto FightGirl = Player->SetFightGirl();
	ui->ClearLog();

	if (FightGirl == nullptr)
	{
		ui->PrintLog("\x1b[90m⚙️ 시스템: 오늘은 이만 물러나기로 했습니다.\x1b[0m");
		UIManager::WaitKey(ui);
		return;
	}

	ui->ClearMainViewport();
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {
		{FightGirl->GetImageKey(), 5, 0, true},
		{BattleGirl->GetImageKey(), 97, 0, false} 
	}));

	ui->PrintLog(script.Get("CITY_BATTLE_TITLE"));
	// 🚨 수정완료: 양쪽 모두 GetColoredName() 넘기기
	ui->PrintLog(script.GetFormatStr("CITY_BATTLE_VS", {FightGirl->GetColoredName(), BattleGirl->GetColoredName()}));
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
    
	ui->ClearMainViewport(); 
    
	if (randomIndex < 40) 
	{
		if (BattleGirl->GetName() == "평범녀")
		{
			ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {})); 
			// 🚨 GetColoredName() 적용
			ui->PrintLog(script.GetFormatStr("CITY_CONTACT_FAIL", {BattleGirl->GetColoredName()}));
			UIManager::WaitKey(ui);
			return ;
		}
        
		BattleGirl->SetLevel(1);
		BattleGirl->SetMaxHp(200);
		BattleGirl->SetCurrentHp(200);
		BattleGirl->SetAttack(30);
        
		Player->AddGirlFrends(BattleGirl);
		Girls.erase(remove(Girls.begin(), Girls.end(), BattleGirl), Girls.end());
        
		ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {{BattleGirl->GetImageKey(), 50, 0, false}}));
		// 🚨 GetColoredName() 적용
		ui->PrintLog(script.GetFormatStr("CITY_CONTACT_SUCCESS", {BattleGirl->GetColoredName()}));
	} else {
		ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
		// 🚨 GetColoredName() 적용
		ui->PrintLog(script.GetFormatStr("CITY_CONTACT_FAIL", {BattleGirl->GetColoredName()}));
	}
    
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
