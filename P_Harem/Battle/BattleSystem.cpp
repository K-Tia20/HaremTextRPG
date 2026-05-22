// BattleSystem.cpp

#include "BattleSystem.h"



int C_BattleSystem::CalculateDamage(int Attack, int Affinity) const
{
	int Damage = (int)floor(Attack * (10.0f / (10 + Affinity)));
	return Damage > 0 ? Damage : 1;
	// 친밀도에 따라 데미지가 감소하며 최소 데미지는 1로 설정;
}

void C_BattleSystem::Attack(C_Creature* Attacker, C_Creature* Defenser)
{
	int Damage = CalculateDamage(Attacker->GetAttack(), Defenser->GetAffinity());
	std::cout << Attacker->GetName() << "의 공격! " << std::endl;
	Sleep(1000); 
	std::cout << Defenser->GetName() << "은(는) " << Damage << "의 데미지를 입었다!" << std::endl;
	Defenser->TakeDamage(Damage);
}

