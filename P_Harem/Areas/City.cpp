
#include "../Framework/Framework.h"
#include "../Battle/BattleSystem.h"
#include "../Areas/Area.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "City.h"


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
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
	//Girls.push_back(make_shred</*히로인들 클래스*/>());
}

void C_City::SelectMenu()
{
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		// 랜덤 인 카운터 만들기
		break;
	case 2:
		// 지역이동 만들기
		break;
	default:
		break;
	}
}

void C_City::MoveArea()
{
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		// 상점으로
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
	Battle->Battle(Player->SetFightGirl(), /*랜덤한 여성*/);
}
