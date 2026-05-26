
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "Shop.h"

C_Shop::C_Shop(C_World* world)
{
	World = world;

	Player = World->GetPlayer();
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

}

void C_Shop::Exit()
{
	World->GotoCity();
}
