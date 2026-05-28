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

class C_Creature
{
protected:
	std::string Name;
	C_Stile Stile;
	int Level = 1; // Affinity를 경험치로 계산
	int Affinity, MaxHp, CurrentHp, Attack;

public:
	C_Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack);
	// 초기 입력값: 이름, 스타일, 최대 체력, 공격력;

	std::string GetName() const { return Name; }
	C_Stile GetStile() const { return Stile; }
	int GetLevel() const { return Level; }
	int GetAffinity() const { return Affinity; }
	int GetMaxHp() const { return MaxHp; }
	int GetCurrentHp() const { return CurrentHp; }
	int GetAttack() const { return Attack; }

	void TakeDamage(int Damage); // Damage를 입력하면 CurrentHp를 Damage만큼 감소시키는 함수;
	bool IsDefeated() const { return CurrentHp <= 0; } // Creature가 쓰러졌는지 확	인하는 함수;

	void SetLevel(int SetLevel) { Level = SetLevel; }
	void SetAffinity(int SetAffinity) { Affinity = SetAffinity; }
	void SetMaxHp(int SetMaxHp) { MaxHp = SetMaxHp; }
	void SetCurrentHp(int SetCurrentHp) { CurrentHp = SetCurrentHp; }
	void SetAttack(int SetAttack) { Attack = SetAttack; }

	void AddHp(int Add);
	
	// [Visual Binding] 히로인 성격에 따른 동적 이미지 매핑
    virtual std::string GetImageKey() const {
        if (Stile == C_Stile::HotGirl) return "CH_Red";
        if (Stile == C_Stile::IceGirl) return "CH_Blue";
        if (Stile == C_Stile::GrassGirl) return "CH_Green";
        return "CH_Normal";
    }

	void SetEnemyStat(int level);
};
