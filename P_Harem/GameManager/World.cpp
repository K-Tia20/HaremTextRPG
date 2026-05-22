
#include "../Framework/Framework.h"
#include "../Player/Player.h"
#include "../Areas/Area.h"
#include "../Areas/City.h"
#include "World.h"

using namespace std;

C_World::C_World()
{
	// 플레이어 생성
	Player = make_shared<C_Player>();
	Areas[WorldArea::City] = make_shared<C_City>(this);
}

void C_World::MainMenu()
{
	// TODO :
	// 플레이어 입력을 받아서 게임을 진행할지 나갈지 선택

	// 로그적어주세요

	int choice = Player->Input<int>();
	switch (choice)
	{
	case 1:
	{
		// 시작
		WS = WorldState::NewGame;
		break;
	}
	default:
		// 종료
		WS = WorldState::QuitGame;
		break;
	}
}

void C_World::NewGame()
{
	// TODO :
	// 게임새로 시작하면 진행되는 로직들 여기서 호출
	// (ex : 플레이어 이름 설정, 히로인 설정 등)
}

void C_World::StartGame()
{ 
	// TODO :
	// 설정이 끝나면 이곳에서 시작지점으로 이동
	// (ex : Area를 다른 곳으로 이동, 아니면 시작 튜토리얼 전투실행)
}

C_Area::C_Area()
{
}

void C_World::Update()
{
	switch (WS)
	{
	case WorldState::MainMenu:
	{
		MainMenu();
		break;
	}
	case WorldState::NewGame:
	{
		NewGame();
		break;
	}
	case WorldState::StartGame:
	{
		StartGame();
		break;
	}
	case WorldState::InProgress:
	{
		// TODO : 현재 에리어에서 게임 진행
	}
	case WorldState::QuitGame:
	{
		IsRunning = false;
		break;
	}
	default:
	{
		cout << "오류 발생...메인으로 돌아갑니다." << endl;
		WS = WorldState::MainMenu();
		break;
	}
	}
}

void C_Area::SetWorld(C_World* world)
{
}

void C_World::SetName()
{
	// TODO :
	// Player클래스에 있는 Input함수를 사용해서 값을 받아서 이름설정
}

void C_World::SetGirlFrends()
{
	// Set(여성)함수를 사용해서 첫 여친 설정

	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		// TODO : 셋테토걸
		break;
	case 2:
		// TODO : 셋쿨미녀걸
		break;
	case 3:
		// TODO : 셋초식걸
		break;
	default:
		// 다른 숫자 입력시 다시해달라 텍스트 걸고 루프
		break;
	}
}

void C_World::SetTetoGirl()
{
}

void C_World::SetCoolPretyGirl()
{
}

void C_World::SetChosicGirl()
{
}
