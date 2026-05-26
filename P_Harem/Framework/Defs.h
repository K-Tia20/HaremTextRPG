#pragma once

#include <iostream>

using namespace std;

class Creature
{
public:
	int Level = 1; // 현재 플레이어 레벨
	int CurrentAffinity = 0; // 현재 플레이어 호감도(경험치)
	int Hp = 200; // 현재 체력
	int MaxHp = 200; // 최대 체력
	int Attack = 30; // 공격력

	int RequiredAffinity[9] = { 100, 100, 150, 150, 200, 250, 300, 300, 300 };
	// 다음 레벨로 가기 위해 필요한 호감도(경험치)

	int GetRequiredAffinity(); // 현재 레벨에서 다음 레벨로 가기 위한 필요 호감도(경험치) 반환
	void GainAffinity(int Affinity); // 호감도(경험치) 획득하는 함수
	void CheckLevelUp(); // 레벨업 가능한지 검사하는 함수
	void LevelUp(); // 실제 레벱업 처리
};