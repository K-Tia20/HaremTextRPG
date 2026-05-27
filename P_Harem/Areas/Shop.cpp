
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "../UI/ScriptManager.h"
#include "../UI/ImageManager.h"
#include "../Objects/Items/Item.h"
#include "Shop.h"

using namespace std;

C_Shop::C_Shop(C_World* world)
{
	World = world;
	Player = World->GetPlayer();

    // 아이템 객체 생성 시 이름은 내부 ID처럼 활용하고, 
    // 실제 출력은 Scenario.txt에서 제어할 수도 있지만 
    // 여기서는 팀원들의 생성 로직을 존중하여 이모지만 포함합니다.
	Items.push_back(make_shared<C_Items>("🍱 보글보글 된장찌개", 20000, 50, ItemType::Heal));
	Items.push_back(make_shared<C_Items>("🍖 육즙팡팡 삼겹살 1.5인분", 50000, 100, ItemType::Heal));
	Items.push_back(make_shared<C_Items>("🍗 오늘밤은 치킨이닭", 100000, 10, ItemType::Power));
}

void C_Shop::SelectMenu()
{
    auto ui = World->GetUI();
    auto& script = C_ScriptManager::GetInstance();
    if (!ui) return;

    // [Visual Upgrade] 상점 외경 출력
    ui->ClearMainViewport();
    ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_YogerPresso_O", {}));

    // [Data-Driven] 상점 메인 레이아웃
    ui->PrintLog(script.Get("UI_DIVIDER_BLUE"));
    ui->PrintLog(script.Get("SHOP_HEADER"));
    ui->PrintLog(script.Get("UI_DIVIDER_BLUE"));
    ui->PrintLog(script.GetFormatStr("SHOP_WALLET", {to_string(Player->GetMonny())}));
    ui->PrintLog(script.Get("UI_LINE_THIN"));
    ui->PrintLog(script.Get("SHOP_MENU_1"));
    ui->PrintLog(script.Get("SHOP_MENU_2"));

	int choice = Player->InputInt();
    ui->ClearLog(); 

	switch (choice)
	{
	case 1: 
        SS = ShopState::Purchase; 
        break;
	case 0:
        SS = ShopState::Exit; 
        break;
	default: 
        ui->PrintLog("시스템: 잘못된 선택입니다.");
        UIManager::WaitKey(ui);
        break;
	}
}

void C_Shop::Update()
{
    bool isLooping = true;
	while (isLooping)
	{
		switch (SS)
		{
		case ShopState::SelectMenu: SelectMenu(); break;
		case ShopState::Purchase: Purchase(); break;
		case ShopState::Exit: Exit(); isLooping = false; break;
		}
	}
}

void C_Shop::Purchase()
{
    auto ui = World->GetUI();
    auto& script = C_ScriptManager::GetInstance();
    if (!ui) return;

    // [Visual Upgrade] 상점 내경(쇼핑 중) 출력
    ui->ClearMainViewport();
    ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_YogerPresso_I", {}));

	ui->PrintLog(script.GetFormatStr("SHOP_LIST_TITLE", {to_string(Player->GetMonny())}));

	int i = 1;
	for (std::shared_ptr<C_Items> Item : Items)
	{
        ui->PrintLog(script.GetFormatStr("SHOP_ITEM_FORMAT", {
            to_string(i), 
            Item->GetItem().Name, 
            to_string(Item->GetItem().Price)
        }));
		i++;
	}
    ui->PrintLog(script.Get("UI_LINE_THIN"));
    ui->PrintLog("0. \x1b[90m뒤로 가기\x1b[0m");

	int choice = Player->InputInt();
    ui->ClearLog(); 

	if (choice == 0) { SS = ShopState::SelectMenu; return; }

	choice -= 1;
	if (choice < 0 || choice >= static_cast<int>(Items.size()))
	{
        ui->PrintLog("\x1b[31m시스템: 그런 메뉴는 없습니다.\x1b[0m");
		SS = ShopState::SelectMenu;
		return;
	}

    ui->PrintLog(script.GetFormatStr("SHOP_ASK_BUY", {Items[choice]->GetItem().Name}));

	switch (Player->InputInt())
	{
	case 1:
	{
		if (Player->GetMonny() < Items[choice]->GetItem().Price)
		{
            ui->PrintLog(script.Get("SHOP_LACK_MONEY"));
			break;
		}
        ui->PrintLog(script.GetFormatStr("SHOP_BUY_SUCCESS", {Items[choice]->GetItem().Name}));
        // [SOUND] 여기에 구매 성공 효과음을 추가하세요 (예: CSoundManager::GetInstance().PlaySFX("Buy.wav");)
		Player->SubMoney(Items[choice]->GetItem().Price);
		Player->AddItem(Items[choice]);
        
        // [Visual Upgrade] 즉각적인 소지금 갱신 연출
        World->SyncUI();
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
    World->SetHubBG("BG_City");
	World->GotoCity();
}
