// BattleSystem.h

#pragma once

#include <functional>
#include "../Framework/Framework.h"
#include "../Creature/Creature.h"


using OnBattleSystemDelegate = std::function<void(std::string, int, std::string)>; 
// 공격 대사와 데미지를 전달하는 델리게이트 타입;

class C_BattleSystem
{
private:



public:

	OnBattleSystemDelegate OnAttack; // 공격 대사와 데미지를 전달하는 델리게이트;
	int CalculateDamage(int Attack, int Defense) const; // 데미지 계산 함수;
	float StileMultiplier(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const; // 스타일 상성 계산 함수;
	void Attack(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser); // 공격을 실행시키는 함수;
	void Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy); // 전투 로직을 처리하는 함수;
};

