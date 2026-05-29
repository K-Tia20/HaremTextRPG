// BattleSystem.cpp

#include "BattleSystem.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Objects/Items/Item.h"
#include "../Creature/Boss.h"
#include "../UI/ImageManager.h"
#include "../UI/LogSystem.h"
#include "../UI/ScriptManager.h"

#include <cmath>

#include "../UI/UIManager.h"

C_LevelSystem LevelSystem; 

C_BattleSystem::C_BattleSystem(C_World* world)
{
	World = world;
	W_Player = world->GetPlayer();
}

int C_BattleSystem::CalculateDamage(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const
{
	if (!Attacker || !Defenser) return 0;
    
	int Damage;
    float multiplier = StileMultiplier(Attacker, Defenser);

	if (multiplier == 2.0f) Damage = (int)floor(Attacker->GetAttack() * 1.5f);
	else if (multiplier == 0.0f) Damage = (int)floor(Attacker->GetAttack() * 0.5f);
	else Damage = Attacker->GetAttack();
    
	return Damage;
}

float C_BattleSystem::StileMultiplier(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const
{
    if (!Attacker || !Defenser) return 1.0f;
	C_Stile AttackerStile = Attacker->GetStile();
	C_Stile DefenserStile = Defenser->GetStile();
    
	if ((AttackerStile == C_Stile::HotGirl && DefenserStile == C_Stile::IceGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefenserStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefenserStile == C_Stile::HotGirl))
	{
		return 0.0f; 
	}
	else if ((AttackerStile == C_Stile::HotGirl && DefenserStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefenserStile == C_Stile::HotGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefenserStile == C_Stile::IceGirl))
	{
		return 2.0f; 
	}
	return 1.0f;
}

void C_BattleSystem::Attack(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser)
{
	if (!Attacker || !Defenser) return;
	int Damage = CalculateDamage(Attacker, Defenser);

	if (UseItem) {
		if (UseItem->GetItem().Type == ItemType::Power && PlayerTurn) {
			Damage += UseItem->GetItem().Value;
			//  GetColoredName() 적용
			if (OnUseItem) OnUseItem(Attacker->GetColoredName(), 1, UseItem->GetItem().Value);
		} else if (UseItem->GetItem().Type == ItemType::Defence && PlayerTurn) {
			Damage -= UseItem->GetItem().Value;
			if (Damage < 0) Damage = 0;
			//  GetColoredName() 적용
			if (OnUseItem) OnUseItem(Attacker->GetColoredName(), 2, UseItem->GetItem().Value);
		}
	}

	//  GetColoredName() 적용
	if (OnAttack) OnAttack(Attacker->GetColoredName());
	if (OnHit) OnHit(Defenser->GetColoredName(), Damage, (int)StileMultiplier(Attacker, Defenser));
	
	Defenser->TakeDamage(Damage);
	UseItem = nullptr;
}

void C_BattleSystem::Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy)
{
    if (!Player || !Enemy) return;
	
	Enemy->SetEnemyStat(W_Player->GetHighlevel());
	
	while (!Player->IsDefeated() && !Enemy->IsDefeated())
	{
		if (rand() % 100 < 60) // 40% 확률로 아이템 사용 시도
		{
			UseItem = W_Player->UsingItem();
		}

        if (UseItem && UseItem->GetItem().Type == ItemType::Heal) {
            Player->AddHp(UseItem->GetItem().Value);

			if (OnUseItem) OnUseItem(Player->GetColoredName(), 3, UseItem->GetItem().Value);
        }

		Attack(Player, Enemy); 
        PlayerTurn = !PlayerTurn;
        Sleep(1000); // 연출을 위해 1초 대기

		if (Enemy->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Enemy->GetColoredName());
			LevelSystem.GainAffinity(Player, 50); 
			break;
		}

		Attack(Enemy, Player); 
        PlayerTurn = !PlayerTurn;
        Sleep(1000);

		if (Player->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Player->GetColoredName());
			break;
		}
	}
}

void C_BattleSystem::BossBattle(std::shared_ptr<C_Creature> Player)
{
	if (!Player) return;

	std::shared_ptr<C_Creature> Boss = std::make_shared<C_Boss>();

	BossBattleIntro(Player, Boss);

	PlayerTurn = false;

	while (!Boss->IsDefeated())
	{
		Attack(Boss, Player); 
		PlayerTurn = !PlayerTurn;
		Sleep(1000); // 연출을 위해 1초 대기

		if (Player->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Player->GetColoredName());

			W_Player->RemoveGirlFriend(Player);

			if (W_Player->GetGirlFrends().empty())
			{
				World->BadEnd();
				
				return;
			}

			Player = W_Player->SetFightGirl();

			if (!Player)
			{
				World->BadEnd();
				return;
			}
		}

		if (rand() % 100 < 60) // 40% 확률로 아이템 사용 시도
		{
			UseItem = W_Player->UsingItem();
		}

		if (UseItem && UseItem->GetItem().Type == ItemType::Heal) {
			Player->AddHp(UseItem->GetItem().Value);

			if (OnUseItem) OnUseItem(Player->GetColoredName(), 3, UseItem->GetItem().Value);
		}
		
		Attack(Player, Boss); 
		PlayerTurn = !PlayerTurn;
		Sleep(1000);
		
		if (Boss->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Boss->GetColoredName());
			LevelSystem.GainAffinity(Player, 50);
			World->RealEnd();
			break;
		}
	}
}

void C_BattleSystem::BossBattleIntro(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Boss)
{
	auto ui = World->GetUI();

	if (!ui) return;

	auto& script = C_ScriptManager::GetInstance();
	auto& img = C_ImageManager::GetInstance();

	ui->ClearMainViewport();
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO0"), 18, 100);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
	Sleep(500);
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO1"), 18, 100);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
	Sleep(500);
	ui->ClearMainViewport();
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO2"), 18, 100);
	Sleep(500);
	ui->ClearMainViewport();
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO3"), 18, 100);
	Sleep(500);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {}));
	Sleep(500);
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO4"), 18, 100);
	Sleep(500);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Boss", {}));
	Sleep(500);
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO5"), 18, 100);
	Sleep(500);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Boss", {}));
	Sleep(500);
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO6"), 18, 100);
	Sleep(500);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Boss", {}));
	Sleep(500);
	ui->CenteredTypeLog(script.Get("BATTLE_BOSS_INTRO7"), 18, 100);
	Sleep(500);
	ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Boss", {}));
	Sleep(500);

	UIManager::WaitKey(ui);

	std::string layeredImg = C_ImageManager::GetInstance().GetLayeredImage("BG_HunPo", {
	{"CH_Hanma", 50, 0, false} // {캐릭터키, X위치(%), Y위치, 좌우반전여부}
		});
	ui->DrawImage(layeredImg);

}
