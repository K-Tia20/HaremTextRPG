
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Objects/Items/Item.h"
#include "Shop.h"

C_Shop::C_Shop(C_World* world)
{
	World = world;

	Player = World->GetPlayer();

	Items.push_back(std::make_shared<C_Items>("된장찌개", 2, 50, ItemType::Heal));
	Items.push_back(std::make_shared<C_Items>("삼겹살 1.5인분", 5, 100, ItemType::Heal));
	Items.push_back(std::make_shared<C_Items>("오늘 저녁은 치킨이닭!", 10, 10, ItemType::Power));
	Items.push_back(std::make_shared<C_Items>("강철의 영약", 2, 20, ItemType::Defence));
}

void C_Shop::SelectMenu()
{
	int choice = Player->Input<int>();

	switch (choice)
	{
	case 1:
		// 구매
		SS = ShopState::Purchase;
		break;
	default:
		SS = ShopState::Exit;
		// 나가기
		break;
	}
}

void C_Shop::Update()
{
	switch (SS)
	{
	case ShopState::SelectMenu:
		SelectMenu();
		break;
	case ShopState::Purchase:
		Purchase();
		break;
	case ShopState::Exit:
		Exit();
		break;
	default:
		break;
	}
}

void C_Shop::Purchase()
{
	int i = 1;
	for (std::shared_ptr<C_Items> Item : Items)
	{
		std::cout << i << ". "
			<< Item->GetItem().Name
			<< "\t가격: " << Item->GetItem().Price
			<< std::endl;

		i++;
	}

	std::cout << "0. 나가기" << std::endl;

	int choice = Player->Input<int>();

	if (choice == 0)
	{
		SS = ShopState::SelectMenu;
		return;
	}

	choice -= 1;

	if (choice < 0 || choice >= static_cast<int>(Items.size()))
	{
		std::cout << "잘못된 입력입니다." << std::endl;
		SS = ShopState::SelectMenu;
		return;
	}

	std::cout << Items[choice]->GetItem().Name << "을(를) 구매하시겠습니까?" << std::endl;
	std::cout << "1. 예" << std::endl;
	std::cout << "2. 아니요" << std::endl;

	switch (Player->Input<int>())
	{
	case 1:
	{
		if (Player->GetMonny() < Items[choice]->GetItem().Price)
		{
			std::cout << "나는 거지다...살 수 없다." << std::endl;
			break;
		}

		std::cout << "구매완료." << std::endl;
		Player->SubMoney(Items[choice]->GetItem().Price);
		Player->AddItem(Items[choice]);
		break;
	}
	default:
		std::cout << "안 삽니다." << std::endl;
		break;
	}
}

void C_Shop::Exit()
{
	SS = ShopState::SelectMenu;
	World->GotoCity();
}
