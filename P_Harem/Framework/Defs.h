#pragma once

#include "Framework.h"
#include "../Creature/Creature.h"

using namespace std;

using OnLevelTextNumDelegate = std::function<void(std::string, int)>;
// �ؽ�Ʈ�� ���ڸ� �����ϴ� ��������Ʈ;

using OnLevelTextNumNumDelegate = std::function<void(std::string, int, int)>;
// �ؽ�Ʈ�� ���� 2���� �����ϴ� ��������Ʈ;

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
	// ����� ������ ���� ���� ������ ���� ���� �ʿ� ȣ������ ��ȯ�ϴ� �Լ�;
	
	void GainAffinity(std::shared_ptr<C_Creature> Creature, int Affinity) 
	{
		Creature->SetAffinity(Creature->GetAffinity() + Affinity);
		CheckLevelUp(Creature);
		if (OnGainAffinity)
		{
			OnGainAffinity(Creature->GetName(), Creature->GetAffinity(), GetRequiredAffinity(Creature));
			// ȣ����(����ġ)ȹ��� �̸�, ���� ȣ����, ���� ������ ���� ���� �ʿ� ȣ������ ����;
		}
	} 
	// ����� ȣ����(����ġ)�� ������Ű�� �Լ�. ��� �� �������� �������� üũ;


	void CheckLevelUp(std::shared_ptr<C_Creature> Creature)
	{
		if (Creature->GetLevel() < 10 && Creature->GetAffinity() >= GetRequiredAffinity(Creature))
		{
			LevelUp(Creature);
			Creature->SetAffinity(Creature->GetAffinity() - GetRequiredAffinity(Creature));
		}
	}
	// ����� ������ ������ �����ϴ��� Ȯ���ϴ� �Լ�. ������ ������ �����ϸ� �������� �����ϰ�, ���� ȣ������ ����Ͽ� ����;


	void LevelUp(std::shared_ptr<C_Creature> Creature)
	{
		Creature->SetLevel(Creature->GetLevel() + 1);
		Creature->SetMaxHp(Creature->GetMaxHp() + Creature->GetLevel() * 20);
		Creature->SetAttack(Creature->GetAttack() + Creature->GetLevel() * 5);
		Creature->SetCurrentHp(Creature->GetMaxHp());
		if (OnLevelUp)
		{
			OnLevelUp(Creature->GetName(), Creature->GetLevel());
			// �������� �̸��� ������ ����;
		}
	}
	// ������ ���� ���� ������ �����Ų �� �̸��� ������ ����


};
