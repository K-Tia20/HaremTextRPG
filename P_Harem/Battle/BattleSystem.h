// BattleSystem.h

#pragma once

#include "../Framework/Framework.h"
#include "../Creature/Creature.h"
#include "../Framework/Defs.h"
#include "../Inventory/Inventory.h"


using OnBattleTextNumNumDelegate = std::function<void(std::string, int, int)>;
// 텍스트와 숫자 2개를 전달하는 델리게이트;

using OnBattleTextNumDelegate = std::function<void(std::string, int)>;
// 텍스트와 숫자를 전달하는 델리게이트;

using OnBattleTextDelegate = std::function<void(std::string)>;
// 텍스트만 전달하는 델리게이트;

class C_Player;
class C_World;
class C_Items;

class C_BattleSystem
{
private:
	std::shared_ptr<C_Player> W_Player;
	std::shared_ptr<C_Items> UseItem;
	bool PlayerTurn = true;


public:
	C_BattleSystem(C_World* world);

	OnBattleTextNumNumDelegate OnHit; // 피격자의 이름과 데미지와 상성을 전달하는 델리게이트;
	// 상성은 유리하면 2, 불리하면 0, 중립은 1로 전달
	
	OnBattleTextNumNumDelegate OnUseItem; // 아이템 사용시 대상의 이름과 효과, 수치를 전달하는 델리게이트
	// 효과는 공격력 증가면 1, 방어력 증가면 2, 체력 회복은 3으로 전달

	OnBattleTextDelegate OnAttack; // 공격자의 이름을 전달하는 델리게이트;
	OnBattleTextDelegate OnDefeat; // 패배자의 이름을 전달하는 델리게이트;
	int CalculateDamage(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const; // 데미지 계산 함수;
	float StileMultiplier(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser) const; // 스타일 상성 계산 함수;
	void Attack(std::shared_ptr<C_Creature> Attacker, std::shared_ptr<C_Creature> Defenser); // 공격을 실행시키는 함수;
	void Battle(std::shared_ptr<C_Creature> Player, std::shared_ptr<C_Creature> Enemy); // 전투 로직을 처리하는 함수;
};
