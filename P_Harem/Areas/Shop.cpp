
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

    // [Visual Upgrade] 메뉴 구성 시 이모지 활용
	Items.push_back(make_shared<C_Items>("🍱 보글보글 된장찌개", 20000, 50, ItemType::Heal));
	Items.push_back(make_shared<C_Items>("🍖 육즙팡팡 삼겹살 1.5인분", 50000, 100, ItemType::Heal));
	Items.push_back(make_shared<C_Items>("🍗 오늘밤은 치킨이닭", 100000, 10, ItemType::Power));
}

void C_Shop::SelectMenu()
{
    auto ui = World->GetUI();
    if (!ui) return;

    ui->PrintLog("\x1b[96m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
    ui->PrintLog("\x1b[97m      ☕ 요거프레쏘 힐링 카페       \x1b[0m");
    ui->PrintLog("\x1b[96m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m");
    ui->PrintLog("\x1b[90m현재 내 지갑: \x1b[93m" + to_string(Player->GetMonny()) + " 원\x1b[0m");
    ui->PrintLog("\x1b[90m------------------------------------\x1b[0m");
    ui->PrintLog("1. \x1b[97m🛒 아이템 쇼핑하기\x1b[0m");
    ui->PrintLog("2. \x1b[90m🏠 밖으로 나가기 (도시)\x1b[0m");

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
    if (!ui) return;

	ui->PrintLog("\x1b[96m--- 🛒 쇼핑 리스트 (잔고: " + to_string(Player->GetMonny()) + "원) ---\x1b[0m");

	int i = 1;
	for (std::shared_ptr<C_Items> Item : Items)
	{
        ui->PrintLog(to_string(i) + ". " + Item->GetItem().Name);
        ui->PrintLog("   \x1b[90m└ 가격: \x1b[93m" + to_string(Item->GetItem().Price) + " 원\x1b[0m");
		i++;
	}
    ui->PrintLog("\x1b[90m------------------------------------\x1b[0m");
    ui->PrintLog("0. \x1b[90m뒤로 가기\x1b[0m");

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
        ui->PrintLog("\x1b[31m시스템: 그런 메뉴는 없습니다.\x1b[0m");
		SS = ShopState::SelectMenu;
		return;
	}

    ui->PrintLog("\x1b[97m[" + Items[choice]->GetItem().Name + "]\x1b[0m");
    ui->PrintLog("이 메뉴로 주문하시겠습니까?");
    ui->PrintLog("1. 👍 예  2. ✋ 아니요");

	switch (Player->InputInt())
	{
	case 1:
	{
		if (Player->GetMonny() < Items[choice]->GetItem().Price)
		{
            ui->PrintLog("\x1b[31m시스템: 지갑이 텅 비었습니다... (잔액 부족)\x1b[0m");
			break;
		}
        ui->PrintLog("\x1b[92m[ 결제 완료! ]\x1b[0m");
        ui->PrintLog("\x1b[97m\"맛있게 드세요! 감사합니다!\"\x1b[0m");
		Player->SubMoney(Items[choice]->GetItem().Price);
		Player->AddItem(Items[choice]);
		break;
	}
	default:
        ui->PrintLog("시스템: 주문을 취소했습니다.");
		break;
	}
    UIManager::WaitKey(ui);
    ui->ClearLog(); 
}

void C_Shop::Exit()
{
	SS = ShopState::SelectMenu;
	World->GotoCity();
}
