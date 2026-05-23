// BattleSystem.cpp

#include "BattleSystem.h"



int C_BattleSystem::CalculateDamage(int Attack, int Affinity) const
{
	int Damage = (int)floor(Attack * (10.0f / (10 + Affinity)));
	return Damage > 0 ? Damage : 1;
	// 친밀도에 따라 데미지가 감소하며 최소 데미지는 1로 설정;
}

int C_BattleSystem::StileMultiplier(C_Creature* Attacker, C_Creature* Defenser) const
{
	C_Stile AttackerStile = Attacker->GetStile();
	C_Stile DefendsrStile = Defenser->GetStile();
	//공격자와 방어자의 스타일을 가져옴;
	if ((AttackerStile == C_Stile::HotGirl && DefendsrStile == C_Stile::IceGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefendsrStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefendsrStile == C_Stile::HotGirl))
	{
		return 0.5; // 상성 유리: 데미지 2배;
	}
	else if ((AttackerStile == C_Stile::HotGirl && DefendsrStile == C_Stile::GrassGirl) ||
		(AttackerStile == C_Stile::IceGirl && DefendsrStile == C_Stile::HotGirl) ||
		(AttackerStile == C_Stile::GrassGirl && DefendsrStile == C_Stile::IceGirl))
	{
		return 1.5; // 상성 불리: 데미지 절반;
	}
	else
	{
		return 1; // 상성 중립: 데미지 그대로;
	}
}

void C_BattleSystem::Attack(C_Creature* Attacker, C_Creature* Defenser)
{
	int Damage = CalculateDamage(Attacker->GetAttack(), Defenser->GetAffinity()) * StileMultiplier(Attacker, Defenser);
	//데미지 계산 후에 스타일 상성을 곱하고 소수점 버림;
	// 공격 모션 출력 함수 호출 (구현 필요);
	Sleep(1000); // 1초 대기
	std::cout << Defenser->GetName() << "은(는) " << Damage << "의 피해를 입었다!" << std::endl;
	Defenser->TakeDamage(Damage);// 입은 데미지만큼 체력 감소;
}


void C_BattleSystem::Battle(C_Creature* Player, C_Creature* Enemy)
{
	while (!Player->IsDefeated() && !Enemy->IsDefeated())
	{
		Attack(Player, Enemy); // 플레이어가 먼저 공격;
		Sleep(1000); // 1초 대기
		if (Enemy->IsDefeated())
		{
			std::cout << Enemy->GetName() << "을(를) 쓰러뜨렸다!" << std::endl;
			//경험치 및 아이템 보상 함수 호출 (구현 필요);
			break;
		}
		Attack(Enemy, Player); // 적의 공격
		if (Player->IsDefeated())
		{
			std::cout << Player->GetName() << "이(가) 패배했다..." << std::endl;
			// 패배 처리 함수 호출 (구현 필요);
			break;
		}
	}
}