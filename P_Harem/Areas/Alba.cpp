
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "Alba.h"

using namespace std;

C_Alba::C_Alba(C_World* world)
{
	World = world;

	Player = World->GetPlayer();
}

// 알바 메뉴
void C_Alba::SelectMenu()
{
	cout << "--------------------------------------------------" << endl;
	cout << endl;
	cout << "[알바하기]" << endl;
	cout << "1. 쿠팡 물류센터 야간 상하차" << endl;
	cout << "2. 당근마켓 빌런 잡기" << endl;
	cout << "3. 결혼식 가짜 하객 대행" << endl;
	cout << "4. 배민커넥트 / 쿠팡이츠 도보 배달" << endl;
	cout << "5 ~. 알바 안 할래(뒤로가기)" << endl;
	cout << endl;
	cout << "--------------------------------------------------" << endl;

	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		cout << "쿠팡 물류센터 야간 상하차를 완료했습니다!" << endl;
		Player->AddMoney(10);
		break;

	case 2:
		cout << "당근마켓 빌런 잡기를 완료했습니다!" << endl;
		Player->AddMoney(10);
		break;

	case 3:
		cout << "결혼식 가짜 하객 대행을 완료했습니다!" << endl;
		Player->AddMoney(10);
		break;

	case 4:
		cout << "배민커넥트 / 쿠팡이츠 도보 배달을 완료했습니다!" << endl;
		Player->AddMoney(10);
		break;
	default:
		AS = AlbaState::MoveArea;
		break;
	}
}

// 지역 이동
void C_Alba::MoveArea()
{
	AS = AlbaState::SelectMenu;
	World->GotoCity();
}

// 업데이트
void C_Alba::Update()
{
	switch (AS)
	{
	case AlbaState::SelectMenu:
		SelectMenu();
		break;
	case AlbaState::MoveArea:
		MoveArea();
		break;
	}
}