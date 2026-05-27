
#include "../Framework/Framework.h"
#include "../Battle/BattleSystem.h"
#include "../Areas/Area.h"
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Framework/CreatureInclude.h"
#include "City.h"

using namespace std;

// 생성자에서 월드를 받음
C_City::C_City(C_World* world)
{
	C_Area::SetWorld(world);

	Player = World->GetPlayer();

	Battle = make_shared<C_BattleSystem>(world);
	// TODO : Creature적들 전부 생성되면 작업

	Girls.push_back(std::make_shared<And>());
	Girls.push_back(std::make_shared<Chashrin>());
	Girls.push_back(std::make_shared<Codelia>());
	Girls.push_back(std::make_shared<Lina>());
	Girls.push_back(std::make_shared<Marry>());
	Girls.push_back(std::make_shared<Oplier>());
	Girls.push_back(std::make_shared<Zhad>());
	Girls.push_back(std::make_shared<C_Creature>("평범녀", C_Stile::NormalGirl, 200, 30));
}

void C_City::SelectMenu()
{
	std::cout << "메뉴 고르기" << std::endl;
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
	{
		int randomIndex = rand() % 100;

		if (randomIndex < 70)
		{
			// 여자 랜덤 인카운터
			Encounter();
		}
		else
		{
			// 30퍼로 여자 못 만남
			cout << "오늘은 허탕 쳤습니다." << endl;
		}
	}
		break;
	case 2:
		ViewYeuchin();
		break;
	case 3:
		// 지역 이동
		CS = CityState::MoveArea;
		break;
	default:
		// 잘못된 입력 다시!
		break;
	}
}

void C_City::MoveArea()
{
	// 지역을 이동하는 곳 입니다. 로그가 필요합니다.

	cout << "지역 이동" << endl;
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
	{
		// 상점으로 이동
		CS = CityState::SelectMenu;
		World->GotoShop();
		break;
	}
	case 2:
	{
		CS = CityState::SelectMenu;
		World->GotoAlba();
		break;
	}
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
		SelectMenu(); // 메뉴 고르기
		break;
	case CityState::MoveArea:
		MoveArea(); // 지역 이동하기
		break;
	}
}

// 여자 인카운터
void C_City::Encounter()
{
	if (Girls.empty())
	{
		cout << "더 이상 만날 사람이 없습니다." << endl;
		return;
	}

	int randomIndex = rand() % static_cast<int>(Girls.size());

	BattleGirl = Girls[randomIndex];

	auto FightGirl = Player->SetFightGirl();

	if (FightGirl == nullptr)
	{
		cout << "전투를 취소했습니다." << endl;
		return;
	}

	Battle->Battle(FightGirl, BattleGirl);

	// 여친 획득
	if(BattleGirl->IsDefeated()) Gatcha();

	// 여친 방생
	if (FightGirl->IsDefeated())
	{
		Player->RemoveGirlFriend(FightGirl);
	}
}

void C_City::Gatcha()
{
	int randomIndex = rand() % 100;

	if (randomIndex < 30)
	{
		// 싸운여자 획득
		BattleGirl->SetMaxHp(200);
		BattleGirl->SetAttack(30);
		Player->AddGirlFrends(BattleGirl);

		// 지역에서 여자 빼내기
		Girls.erase(std::remove(Girls.begin(), Girls.end(), BattleGirl), Girls.end());
	}
}

void C_City::ViewYeuchin()
{
	std::vector<std::shared_ptr<C_Creature>> ViewYeuchin;

	ViewYeuchin = World->GetPlayer()->GetGirlFrends();

	// 델리게이트로 벡터보내기
	// 
}
