#include "Defs.h" // Creature 클래스 선언이 들어있는 헤더 파일 포함

int Creature::GetRequiredAffinity() // 현재 레벨에서 다음 레벨로 가기 위해 필요한 호감도(경험치) 반환
{
	if (Level >= 10) // 최대 레벨(10) 이상이면
		return 0; // 더 이상 필요한 호감도(경험치)가 없으므로 0 반환

	return RequiredAffinity[Level - 1]; // 현재 레벨에 맞는 필요 호감도(경험치)를 배열에서 꺼내 반환
}
void Creature::GainAffinity(int Affinity) // 호감도(경험치)를 획득하는 함수
{
	if (Level >= 10) // 이미 최대 레벨이면
		return; // 호감도(경험치)를 더 이상 받지 않고 종료

	CurrentAffinity += Affinity; // 전달받은 호감도(경험치)를 현재 호감도(경험치)에 다함
	CheckLevelUp(); // 호감도(경험치)를 얻은 후 레벨업 가능한지 확인
}
void Creature::CheckLevelUp() // 레벨업 가능한지 검사하는 함수
{
	while (Level < 10 && CurrentAffinity >= GetRequiredAffinity()) // 최대 레벨이 아니고, 현재 호감도(경험치)가 필요 호감도(경험치) 이상이면 계속 반복
	{
		CurrentAffinity -= GetRequiredAffinity(); // 레벨에 필요한 호감도(경험치)만큼 차감
		LevelUp(); // 실제 레벨업 처리 실행
	}
}
void Creature::LevelUp() // 실제 레벨업 처리 함수
{
	MaxHp += Level * 20; // 증가한 레벨 기준으로 최대 체력 증가
	Attack += Level * 5; // 증가한 레벨 기준으로 공격력 증가
	Level++; // 최대 체력과 공격력 증가 후 레벨 1 증가
	Hp = MaxHp; // 현재 체력을 최대 체력까지 회복
}