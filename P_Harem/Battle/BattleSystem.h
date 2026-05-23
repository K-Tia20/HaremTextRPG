// BattleSystem.h

#pragma once

#include "../Framework/Framework.h"
#include "../Creature/Creature.h"


class C_BattleSystem
{
private:



public:
	int CalculateDamage(int Attack, int Defense) const; // 데미지 계산 함수;
	int StileMultiplier(C_Creature* Attacker, C_Creature* Defender) const; // 스타일 상성 계산 함수;
	void Attack(C_Creature* Attacker, C_Creature* Defender); // 공격을 실행시키는 함수;
	void Battle(C_Creature* Player, C_Creature* Enemy); // 전투 로직을 처리하는 함수;
};

