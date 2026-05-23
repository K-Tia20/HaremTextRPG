
#include "../Framework/Framework.h"
#include "../Player/Player.h"
#include "../Areas/Area.h"
#include "../Areas/City.h"
#include "../Creature/Creature.h"
#include "World.h"

using namespace std;

C_World::C_World()
{
	// 플레이어 생성
	Player = make_shared<C_Player>();
	//Areas[WorldArea::City] = make_shared<C_City>(this);
}

void C_World::MainMenu()
{
	// TODO :
	// 플레이어 입력을 받아서 게임을 진행할지 나갈지 선택

	// 로그적어주세요
	cout << "TEXT RPG" << endl;
	cout << "선택 : ";

	int choice = Player->Input<int>();
	switch (choice)
	{
	case 1:
	{
		// 시작
		// 로그 적어주세요
		WS = WorldState::NewGame;
		break;
	}
	default:
		// 종료
		// 게임을 종료하는 로그를 적어주세요.
		WS = WorldState::QuitGame;
		break;
	}
}

void C_World::NewGame()
{
	// TODO :
	// 게임새로 시작하면 진행되는 로직들 여기서 호출
	// (ex : 플레이어 이름 설정, 히로인 설정 등)
	// 로그로 꾸며주셔도 됩니다.(안해도 됨)
	
	// Player의 이름이 없으면 SetName()함수 호출
	if (Player->GetName().empty()) SetName();

	// Player의 동료가 설정되지 않았으면 SetGirlFrends()함수 호출
	if (Player->GetGirlFrends().empty()) SetGirlFrends();
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


void C_World::SetName()
{
	// TODO :
	// Player클래스에 있는 Input함수를 사용해서 값을 받아서 이름설정
	// 이름적는 로그가 필요합니다!!
	cout << "히히" << endl;

	string newname = Player->Input<string>();

	// 이름 확인하는 로그 
	cout << "이름 : " << newname << " 으로 하시겠습니까?" << endl;

	cout << "1. 현재 이름으로 진행합니다." << endl;
	cout << "2. 다른 이름으로 진행합니다." << endl;
	cout << endl;
	cout << "선택 : ";

	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
	{
		Player->SetName(newname);
		break;
	}
	default:
	{
		cout << "다른 이름으로 설정합니다." << endl;
		break;
	}
	}
}

void C_World::SetGirlFrends()
{
	// Set(여성)함수를 사용해서 첫 여친 설정
	// 여캐 고르는 로그 적어주세요
	cout << "여자고르기" << endl;
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		SetTetoGirl();
		break;
	case 2:
		SetCoolPretyGirl();
		break;
	case 3:
		SetChosicGirl();
		break;
	default:
		cout << "잘못 된 입력입니다." << endl;
		break;
	}
}

void C_World::SetTetoGirl()
{
	// 테토녀의 정보 로그를 적어주세요
	// 입력 로그를 적어주세요
	switch (Player->Input<int>())
	{
	case 1:
	{
		cout << "당신의 뜨거운 파트너입니다." << endl;	

		// 테토걸클래스를 make_shared의 생성클래스로 바꿔주세요
		shared_ptr<C_Creature> TetoGirl = make_shared<C_Creature>();
		Player->AddGirlFrends(TetoGirl);
		break;
	}
	default:
	{
		cout << "아닙니다." << endl;
		break;
	}
	}
}

void C_World::SetCoolPretyGirl()
{
	// 쿨미녀의 정보 로그를 적어주세요
	// 입력 로그를 적어주세요
	switch (Player->Input<int>())
	{
	case 1:
	{
		cout << "당신의 차가운 파트너입니다." << endl;

		// 쿨미녀클래스를 make_shared의 생성클래스로 바꿔주세요
		shared_ptr<C_Creature> CoolPretyGirl = make_shared<C_Creature>();
		Player->AddGirlFrends(CoolPretyGirl);
		break;
	}
	default:
	{
		cout << "아닙니다." << endl;
		break;
	}
	}
}

void C_World::SetChosicGirl()
{
	// 초식녀의 정보 로그를 적어주세요
	// 입력 로그를 적어주세요
	switch (Player->Input<int>())
	{
	case 1:
	{
		cout << "당신의 풀내나는 파트너입니다." << endl;

		// 초식녀클래스를 make_shared의 생성클래스로 바꿔주세요
		shared_ptr<C_Creature> ChosicGirl = make_shared</*ChosicGirl*/C_Creature>();
		Player->AddGirlFrends(ChosicGirl);
		break;
	}
	default:
	{
		cout << "아닙니다." << endl;
		break;
	}
	}
}

bool C_World::CheckInit()
{
	return false;
}
