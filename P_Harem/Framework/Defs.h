#pragma once

#include <iostream>

using namespace std;

class Creature
{
public:
	int Level = 1; // 현재 플레이어 레벨
	int CurrentExp = 0; // 현재 플레이어 경험치
	int Hp = 200; // 현재 체력
	int MaxHp = 200; // 최대 체력
	int Attack = 30; // 공격력

	int RequiredExp[9] = { 100, 100, 150, 200, 250, 300, 400, 500, 600 };
	// 다음 레벨로 가기 위해 필요한 경험치

	int GetRequiredExp() // 현재 레벨에서 다음 레벨로 가기 위한 필요 경험치 반환
	{
		if (Level >= 10) // 최대 레벨이면
			return 0; // 더 이상 필요한 경험치가 없으므로 0 반환

		return RequiredExp[Level - 1]; // 현재 레벨에 맞는 필요 경험치 반환
	}
	void GainExp(int Exp) // 경험치 획득하는 함수
	{
		if (Level >= 10) // 이미 최대 레벨이면
			return; // 경험치를 더 이상 받지 않고 함수 종료

		CurrentExp += Exp; // 전달 받은 경험치를 현재 경험치에 더함
		CheckLevelUp(); // 경험치를 더한 뒤 레벨업 가능한지 확인
	}
	void CheckLevelUp() // 레벨업 가능한지 검사하는 함수
	{
		// 최대 레벨이 아니고, 현재 경험치가 필요 경험치 이상이면 반복
		while (Level < 10 && CurrentExp >= GetRequiredExp())
		{
			CurrentExp -= GetRequiredExp(); // 레벨업에 필요한 경험치만큼 차감
			LevelUp(); // 실제 레벨업 처리 실행
		}
	}
	void LevelUp() // 실제 레벱업 처리
	{
		MaxHp += Level * 20; // 레벨업 시 MaxHp가 레벨당 x 20 즉, 2레벨에 최대 체력 220(+20) / 3레벨에 최대 체력 260(+40)
		Attack += Level * 5; // 레벨업 시 Attack가 레벨당 x 5 즉, 2레벨에 Attack 35(+5) / 3레벨에 45(+10)
		Level++; // 위의 값을 반영한 뒤 레벨 업
		Hp = MaxHp; // 레벨업 시 Hp가 MaxHp로
	}
};