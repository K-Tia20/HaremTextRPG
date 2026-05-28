// BattleSystem.cpp

#include "BattleSystem.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Objects/Items/Item.h"
#include <cmath>

C_LevelSystem LevelSystem; 

C_BattleSystem::C_BattleSystem(C_World* world)
{
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

    // [Stability Fix & Team Intent Respect] 아이템 타입별 로직 내부에 델리게이트 배치
    if (UseItem) {
        if (UseItem->GetItem().Type == ItemType::Power && PlayerTurn) {
            Damage += UseItem->GetItem().Value;
            if (OnUseItem) OnUseItem(Attacker->GetName(), 1, UseItem->GetItem().Value);
        } else if (UseItem->GetItem().Type == ItemType::Defence && PlayerTurn) {
            Damage -= UseItem->GetItem().Value;
            if (Damage < 0) Damage = 0;
            if (OnUseItem) OnUseItem(Attacker->GetName(), 2, UseItem->GetItem().Value);
        }
    }

	if (OnAttack) OnAttack(Attacker->GetName());

	if (OnHit) OnHit(Defenser->GetName(), Damage, (int)StileMultiplier(Attacker, Defenser));
	
	Defenser->TakeDamage(Damage);
	UseItem = nullptr;
}

void C_BattleSystem::Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy)
{
    if (!Player || !Enemy) return;

	while (!Player->IsDefeated() && !Enemy->IsDefeated())
	{
		if (rand() % 100 < 40) // 40% 확률로 아이템 사용 시도
		{
			UseItem = W_Player->UsingItem();
		}

        if (UseItem && UseItem->GetItem().Type == ItemType::Heal) {
            Player->AddHp(UseItem->GetItem().Value);

			if (OnUseItem) OnUseItem(Player->GetName(), 3, UseItem->GetItem().Value);
        }

		Attack(Player, Enemy); 
        PlayerTurn = !PlayerTurn;
        Sleep(1000); // 연출을 위해 1초 대기

		if (Enemy->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Enemy->GetName());
			LevelSystem.GainAffinity(Player, 50); 
			break;
		}

		Attack(Enemy, Player); 
        PlayerTurn = !PlayerTurn;
        Sleep(1000);

		if (Player->IsDefeated())
		{
			if (OnDefeat) OnDefeat(Player->GetName());
			break;
		}
	}
}
