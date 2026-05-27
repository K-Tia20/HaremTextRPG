
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "../Objects/Items/Item.h"
#include "Shop.h"

using namespace std;

C_Shop::C_Shop(C_World* world)
{
	World = world;
	Player = World->GetPlayer();

    // C_Items 생성자는 (이름, 가격, 효능치) 3개의 인자를 받습니다.
	Items.push_back(make_shared<C_Items>("된장찌개", 2, 50));
	Items.push_back(make_shared<C_Items>("삼겹살 1.5인분", 5, 100));
	Items.push_back(make_shared<C_Items>("치킨이닭", 10, 10));
}

void C_Shop::SelectMenu()
{
    auto ui = World->GetUI();
    ui->PrintLog("--- 보따리 상점 ---");
    ui->PrintLog("1. 아이템 구매하기");
    ui->PrintLog("2. 나가기 (도시로)");

	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1:
		SS = ShopState::Purchase;
		break;
	default:
		SS = ShopState::Exit;
		break;
	}
}

void C_Shop::Update()
{
	switch (SS)
	{
	case ShopState::SelectMenu: SelectMenu(); break;
	case ShopState::Purchase: Purchase(); break;
	case ShopState::Exit: Exit(); break;
	}
}

void C_Shop::Purchase()
{
    auto ui = World->GetUI();
	ui->PrintLog("--- 아이템 목록 ---");

	int i = 1;
	for (std::shared_ptr<C_Items> Item : Items)
	{
        ui->PrintLog(to_string(i) + ". " + Item->GetItem().Name + " (가격: " + to_string(Item->GetItem().Price) + "G)");
		i++;
	}
    ui->PrintLog("0. 뒤로 가기");

	int choice = Player->InputInt();
    ui->ClearLog(); 

	if (choice == 0)
	{
		SS = ShopState::SelectMenu;
		return;
	}

	choice -= 1;
	if (choice < 0 || choice >= static_cast<int>(Items.size()))
	{
        ui->PrintLog("시스템: 잘못된 선택입니다.");
		SS = ShopState::SelectMenu;
		return;
	}

    ui->PrintLog("[" + Items[choice]->GetItem().Name + "] 을(를) 구매하시겠습니까?");
    ui->PrintLog("1. 예  2. 아니요");

	switch (Player->InputInt())
	{
	case 1:
	{
		if (Player->GetMonny() < Items[choice]->GetItem().Price)
		{
            ui->PrintLog("시스템: 골드가 부족합니다.");
			break;
		}
        ui->PrintLog("시스템: 구매가 완료되었습니다!");
		Player->SubMoney(Items[choice]->GetItem().Price);
		Player->AddItem(Items[choice]);
		break;
	}
	default:
        ui->PrintLog("시스템: 구매를 취소했습니다.");
		break;
	}
    ui->ClearLog(); 
}

void C_Shop::Exit()
{
	SS = ShopState::SelectMenu;
	World->GotoCity();
}
