#pragma once

#include <iostream>

using namespace std;

class Creature
{
public:
	int Level = 1; // 현재 플레이어 레벨
	int CurrentExp = 0; // 현재 플레이어 경험치
	int MaxHp = 200; // 최대 체력
	int Attack = 30; // 공격력

	int RequiredExp[9] = {100, 100, 150, 200, 250, 300, 400, 500, 600};

	int GetRequiredExp() // 지금 레벨에 필요한 경험치
	{
		if (Level >= 10)
			return 0;
		
		return RequiredExp[Level - 1];
	}
	void GainExp(int Exp) // 경험치 획득
	{
		if (Level >= 10)
			return;
		
		CurrentExp += Exp;
		CheckLevelUp();
	};
	void CheckLevelUp() // 레벨업 가능한지 검사
	{
		while (Level < 10 && CurrentExp >= GetRequiredExp())
		{
			CurrentExp -= GetRequiredExp();
			LevelUp();
		}
	}
	void LevelUp() // 실제 레벱업 처리
	{
	};