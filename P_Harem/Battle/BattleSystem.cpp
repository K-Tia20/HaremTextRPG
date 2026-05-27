// BattleSystem.cpp

#include "BattleSystem.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Objects/Items/Item.h"

C_LevelSystem LevelSystem; // 레벨 시스템 인스턴스 생성

C_BattleSystem::C_BattleSystem(C_World* world)
{
	W_Player = world->GetPlayer();
}

int C_BattleSystem::CalculateDamage(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const
{
	int Damage;

	if (StileMultiplier(Attacker, Defenser) == 2)
	{
		Damage = (int)floor(Attacker->GetAttack() * 1.5);
	}
	else if (StileMultiplier(Attacker, Defenser) == 0)
	{
		Damage = (int)floor(Attacker->GetAttack() * 0.5);
	}
	else
	{
		Damage = Attacker->GetAttack();
	}
	return Damage;
}

float C_BattleSystem::StileMultiplier(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const
{
	C_Stile AttackerStile = Attacker->GetStile();
	C_Stile DefenserStile = Defenser->GetStile();
	//공격자와 방어자의 스타일을 가져옴;
	if ((AttackerStile == C_Stile::HotGirl && DefenserStile == C_Stile::IceGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefenserStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefenserStile == C_Stile::HotGirl))
	{
		return 0; // 상성 불리: 데미지 절반;
	}
	else if ((AttackerStile == C_Stile::HotGirl && DefenserStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefenserStile == C_Stile::HotGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefenserStile == C_Stile::IceGirl))
	{
		return 2; // 상성 유리 : 데미지 1.5배;
	}
	else
	{
		return 1; // 상성 동일: 데미지 그대로;
	}
}

void C_BattleSystem::Attack(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser)
{
	int Damage = 0;

	if (UseItem->GetItem().Type == ItemType::Power && PlayerTurn)
	{
		Damage = CalculateDamage(Attacker, Defenser) + UseItem->GetItem().Value;
	}
	else if (UseItem->GetItem().Type == ItemType::Defence && PlayerTurn)
	{
		Damage = CalculateDamage(Attacker, Defenser) - UseItem->GetItem().Value;
	}
	else
	{
		Damage = CalculateDamage(Attacker, Defenser);
	}

	//공격력에 스타일 상성을 곱하고 소수점 버림;
	if (OnAttack)
	{
		OnAttack(Attacker->GetName());
	}
	if (OnHit)
	{
		OnHit(Defenser->GetName(), Damage, (int)StileMultiplier(Attacker, Defenser));
		// 공격자의 이름, 데미지, 상성을 전달;
		// Multiplier(상성)은 유리하면2 불리하면0 중립이면1;
	}
	Defenser->TakeDamage(Damage);// 입은 데미지만큼 체력 감소;

	UseItem = nullptr;
}


void C_BattleSystem::Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy)
{
	while (!Player->IsDefeated() && !Enemy->IsDefeated())
	{
		if (rand() % 100 < 40)
		{
			UseItem = W_Player->UsingItem();
		}

		if (UseItem->GetItem().Type == ItemType::Heal)
		{
			Player->AddHp(UseItem->GetItem().Value);
		}

		Attack(Player, Enemy); // 플레이어가 먼저 공격;
		PlayerTurn = !PlayerTurn;
		Sleep(1000); // 1초 대기
		if (Enemy->IsDefeated())
		{
			if (OnDefeat)
			{
				OnDefeat(Enemy->GetName()); // 쓰러뜨린 적의 이름 전달;
			}
			LevelSystem.GainAffinity(Player, 50); // 경험치 획득;
			break;
		}
		Attack(Enemy, Player); // 적의 공격
		if (Player->IsDefeated())
		{
			if (OnDefeat)
			{
				OnDefeat(Player->GetName());
			}
			// 패배 처리 함수 호출 (구현 필요);
			break;
		}
		PlayerTurn = !PlayerTurn;
	}
}
