
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Objects/Items/Item.h"
#include "Shop.h"

C_Shop::C_Shop(C_World* world)
{
	World = world;

	Player = World->GetPlayer();

	Items.push_back(std::make_shared<C_Items>("된장찌개", 2, 50));
	Items.push_back(std::make_shared<C_Items>("삼겹살 1.5인분", 5, 100));
	Items.push_back(std::make_shared<C_Items>("오늘 저녁은 치킨이닭!", 10, 10));
	Items.push_back(std::make_shared<C_Items>("강철의 영약", 2, 20));
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
		std::cout << i << ". " << Item  << "\t" << Item->GetItem().Price << std::endl;

		i++;
	}

	int choice = Player->Input<int>() - 1;

	if (choice <= 0) return;

	std::cout << Items[choice - 1]->GetItem().Name << std::endl;

	switch (Player->Input<int>())
	{
	case 1:
	{
		if (Player->GetMonny() < Items[choice - 1]->GetItem().Price)
		{
			std::cout << "나는 거지다...살 수 없다." << std::endl;
			break;
		}
		std::cout << "구매완료." << std::endl;
		Player->SubMoney(Items[choice - 1]->GetItem().Price);

		Player->AddItem(Items[choice - 1]);
	}
		break;
	default:
	{
		std::cout << "안 삽니다." << std::endl;
	}
		break;
	}
}

void C_Shop::Exit()
{
	World->GotoCity();
}
