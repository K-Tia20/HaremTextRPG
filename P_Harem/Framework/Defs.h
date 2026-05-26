#pragma once

#include "Framework.h"
#include "../Creature/Creature.h"

using namespace std;

using OnLevelTextNumDelegate = std::function<void(std::string, int)>;
// 텍스트와 숫자를 전달하는 델리게이트;

using OnLevelTextNumNumDelegate = std::function<void(std::string, int, int)>;
// 텍스트와 숫자 2개를 전달하는 델리게이트;

class C_LevelSystem
{
public:
	OnLevelTextNumNumDelegate OnGainAffinity;
	OnLevelTextNumDelegate OnLevelUp;

	int RequiredAffinity[9] = { 100, 100, 150, 150, 200, 250, 300, 300, 300 };


	int GetRequiredAffinity(std::shared_ptr<C_Creature> Creature) const
	{
		if (Creature->GetLevel() >= 10)
			return 0;
		return RequiredAffinity[Creature->GetLevel() - 1];
	} 
	// 대상의 레벨에 따른 다음 레벨로 가기 위한 필요 호감도를 반환하는 함수;

	
	void GainAffinity(std::shared_ptr<C_Creature> Creature, int Affinity) 
	{
		Creature->SetAffinity(Creature->GetAffinity() + Affinity);
		CheckLevelUp(Creature);
		if (OnGainAffinity)
		{
			OnGainAffinity(Creature->GetName(), Creature->GetAffinity(), GetRequiredAffinity(Creature));
			// 호감도(경험치)획득시 이름, 현재 호감도, 다음 레벨로 가기 위한 필요 호감도를 전달;
		}
	} 
	// 대상의 호감도(경험치)를 증가시키는 함수. 계산 후 레벨업이 가능한지 체크;


	void CheckLevelUp(std::shared_ptr<C_Creature> Creature)
	{
		if (Creature->GetLevel() < 10 && Creature->GetAffinity() >= GetRequiredAffinity(Creature))
		{
			LevelUp(Creature);
			Creature->SetAffinity(Creature->GetAffinity() - GetRequiredAffinity(Creature));
		}
	}
	// 대상이 레벨업 조건을 충족하는지 확인하는 함수. 레벨업 조건을 충족하면 레벨업을 수행하고, 남은 호감도를 계산하여 설정;


	void LevelUp(std::shared_ptr<C_Creature> Creature)
	{
		Creature->SetLevel(Creature->GetLevel() + 1);
		Creature->SetMaxHp(Creature->GetMaxHp() + Creature->GetLevel() * 20);
		Creature->SetAttack(Creature->GetAttack() + Creature->GetLevel() * 5);
		Creature->SetCurrentHp(Creature->GetMaxHp());
		if (OnLevelUp)
		{
			OnLevelUp(Creature->GetName(), Creature->GetLevel());
			// 레벨업시 이름과 레벨을 전달;
		}
	}
	// 레벨에 따른 스펙 증가를 적용시킨 후 이름과 레벨을 전달


};
