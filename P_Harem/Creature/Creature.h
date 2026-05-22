// Creature.h

#pragma once
#include "../Framework/Framework.h"

enum class C_Stile 
{ 
	HotGirl, 
	IceGirl, 
	GrassGirl,
	NormalGirl 
};

class Creature
{
protected:
	std::string Name;
	C_Stile Stile;
	int Level = 1;
	int MaxHp, CurrentHp, Attack;
	int Affinity = 0;

public:
	Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack, int Affinity);
	// 초기 입력값: 이름, 스타일, 최대 체력, 공격력, 친밀도;

	std::string GetName() const { return Name; }
	C_Stile GetStile() const { return Stile; }
	int GetLevel() const { return Level; }
	int GetMaxHp() const { return MaxHp; }
	int GetCurrentHp() const { return CurrentHp; }
	int GetAttack() const { return Attack; }
	int GetAffinity() const { return Affinity; }

	void TakeDamage(int Damage); // Damage를 입력하면 CurrentHp를 Damage만큼 감소시키는 함수;
	bool IsDefeated() const { return CurrentHp <= 0; } // Creature가 쓰러졌는지 확	인하는 함수;

	void SetLevel(int _Level) { Level = _Level; }
	void SetMaxHp(int SetMaxHp) { MaxHp = SetMaxHp; }
	void SetCurrentHp(int SetCurrentHp) { CurrentHp = SetCurrentHp; }
	void SetAttack(int SetAttack) { Attack = SetAttack; }
	void SetAffinity(int SetAffinity) { Affinity = SetAffinity; }
	//이름과 스타일은 Setter가 없으니 필요하면 추가할 것;
	
};

