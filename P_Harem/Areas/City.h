#pragma once

#include "../Framework/Framework.h"
#include "Area.h"

class C_Creature;
class C_BattleSystem;
class C_World;
class Player;

enum class CityState
{
	SelectMenu,
	MoveArea
};

class C_City : public C_Area
{
public:
	C_City(C_World* world);
	C_City();

	// 메뉴고르기
	virtual void SelectMenu() override;
	// 지역이동
	virtual void MoveArea() override;
	// 게임 진행 
	virtual void Update() override;
	// 배틀 진입
	virtual void Encounter() override;

private:
	// 이 지역의 여성들
	std::vector<std::shared_ptr<C_Creature>> Girls;
	// 배틀하기 위한 클래스 생성
	std::shared_ptr<C_BattleSystem> Battle;
	// 플레이어 정보 얻어오기
	std::shared_ptr<C_Player> Player;
	// Area상태
	CityState CS = CityState::SelectMenu;
};