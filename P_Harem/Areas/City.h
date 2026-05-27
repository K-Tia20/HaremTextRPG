#pragma once

#include "../Framework/Framework.h"
#include "Area.h"

class C_Creature;
class C_BattleSystem;
class C_Player;

enum class CityState
{
	SelectMenu,
	MoveArea
};

class C_City : public C_Area
{
public:
	C_City(C_World* world);

	// 메뉴고르기
	virtual void SelectMenu() override;
	// 지역이동
	virtual void MoveArea() override;
	// 게임 진행 
	virtual void Update() override;
	// 배틀 진입
	virtual void Encounter() override;
	// 여친 획득!
	void Gatcha();
	// 여친 확인
	void ViewYeuchin();

private:
	// 이 지역의 여성들
	std::vector<std::shared_ptr<C_Creature>> Girls;
	// 적 정보 얻어오기
	std::shared_ptr<C_Creature> BattleGirl;
	// 배틀하기 위한 클래스 참조 (World에서 빌려옵니다)
	C_BattleSystem* Battle;
	// 플레이어 정보 얻어오기
	std::shared_ptr<C_Player> Player;
	// Area상태
	CityState CS = CityState::SelectMenu;
};