
#include "../Framework/Framework.h"
#include "../Battle/BattleSystem.h"
#include "../Areas/Area.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "City.h"

using namespace std;

using namespace std;

// 생성자에서 World를 받음
C_City::C_City()
{
}

C_City::C_City(C_World* world)
{
	C_Area::SetWorld(world);

	Player = World->GetPlayer();
	// TODO : Creature적들 전부 생성되면 작업

	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
	//Girls.push_back(make_shared</*히로인들 클래스*/>());
}

void C_City::SelectMenu()
{
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
	{
		int randomIndex = rand() % 100;

		if (randomIndex < 70)
		{
			Encounter();
		}
		else
		{
			cout << "오늘은 허탕 쳤습니다." << endl;
		}
	}


void C_City::MoveArea()
{
	// 지역을 이동하는 곳 입니다. 로그가 필요합니다.

	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		// 상점으로 이동
		World->GotoShop();
		break;
	case 2:
		// 아르바이트 구현
		break;
	default:
		cout << "다시 골라주세요..." << endl;
		break;
	}
}

void C_City::Update()
{
	switch (CS)
	{
	case CityState::SelectMenu:
		SelectMenu();
		break;
	case CityState::MoveArea:
		MoveArea();
		break;
	}
}

void C_City::Encounter()
{
	int randomIndex = rand() % Girls.size();
	Battle->Battle(Player->SetFightGirl(), /*랜덤한 여성*/);
}
