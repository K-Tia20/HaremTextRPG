// BattleSystem.h

#pragma once

#include "../Framework/Framework.h"
#include "../Creature/Creature.h"
enum class Stile { HotGirl, IceGirl, GrassGirl, NomalGirl };


class C_BattleSystem
{
private:



public:
	int CalculateDamage(int Attack, int Defense) const; // 데미지 계산 함수;
	void Attack(C_Creature* Attacker, C_Creature* Defender); // 공격을 실행시키는 함수;
};