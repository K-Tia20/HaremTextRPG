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
		// if 대신 while을 써서 한 번에 경험치를 많이 먹어도 연속 레벨업이 되게 방어
		while (Creature->GetLevel() < 10 && Creature->GetAffinity() >= GetRequiredAffinity(Creature))
		{
			// 현재 레벨업에 필요한 경험치를 미리 뺀닷
			Creature->SetAffinity(Creature->GetAffinity() - GetRequiredAffinity(Creature));
			
			// 안전하게 남은 경험치를 보존한 채로 레벨을 올리기
			LevelUp(Creature); 
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
