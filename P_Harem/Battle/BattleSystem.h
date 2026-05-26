// BattleSystem.h

#pragma once

#include "../Framework/Framework.h"
#include "../Creature/Creature.h"


using OnBattleTextEndNumberDelegate = std::function<void(std::string, int)>; 
// 텍스트와 숫자를 전달하는 델리게이트;
using OnBattleTextDelegate = std::function<void(std::string)>;
// 텍스트만 전달하는 델리게이트;


class C_BattleSystem
{
private:



public:

	OnBattleTextEndNumberDelegate OnHit; // 피격자의 이름과 데미지를 전달하는 델리게이트;
	OnBattleTextDelegate OnAttack; // 공격자의 이름을 전달하는 델리게이트;
	OnBattleTextDelegate OnDefeat; // 패배자의 이름을 전달하는 델리게이트;
	int CalculateDamage(int Attack, int Defense) const; // 데미지 계산 함수;
	float StileMultiplier(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const; // 스타일 상성 계산 함수;
	void Attack(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser); // 공격을 실행시키는 함수;
	void Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy); // 전투 로직을 처리하는 함수;
};

