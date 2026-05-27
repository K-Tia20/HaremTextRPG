#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../UI/UIManager.h" 
#include "../UI/ScriptManager.h"
#include "../UI/ImageManager.h"
#include "Alba.h"
#include <windows.h>

using namespace std;

C_Alba::C_Alba(C_World* world)
{
	World = world;
	Player = World->GetPlayer();
}

void C_Alba::SelectMenu()
{
    auto ui = World->GetUI();
    auto& script = C_ScriptManager::GetInstance();
    if (!ui) return;

    // [Data-Driven] Scenario.txt에서 레이아웃 호출
	ui->PrintLog(script.Get("UI_DIVIDER_YELLOW"));
	ui->PrintLog(script.Get("ALBA_HEADER"));
	ui->PrintLog(script.Get("UI_DIVIDER_YELLOW"));
	ui->PrintLog(script.Get("ALBA_MENU_1"));
	ui->PrintLog(script.Get("ALBA_MENU_2"));
	ui->PrintLog(script.Get("ALBA_MENU_3"));
	ui->PrintLog(script.Get("ALBA_MENU_4"));
	ui->PrintLog(script.Get("UI_LINE_THIN"));
	ui->PrintLog(script.Get("ALBA_BACK"));

	int choice = Player->InputInt();
    ui->ClearLog(); 

    string jobName = "";
    string bgName = "";
    int pay = 100000;

	switch (choice)
	{
	case 1: jobName = "상하차 알바"; bgName = "BG_Coupang"; break;
	case 2: jobName = "벌레잡기 알바"; bgName = "BG_Danggun"; break;
	case 3: jobName = "하객대행 알바"; bgName = "BG_Wedding"; break;
	case 4: jobName = "배달 알바"; bgName = "BG_Bemin"; break;
	default: AS = AlbaState::MoveArea; return;
	}

    // [Visual Upgrade] 알바 전용 배경 출력
    ui->ClearMainViewport();
    ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage(bgName, {}));

    // [Data-Driven] 작업 진행 연출
    ui->PrintLog(script.GetFormatStr("ALBA_START", {jobName}));
    for (int i = 1; i <= 5; ++i) {
        string gage = "[";
        for (int j = 0; j < 5; ++j) gage += (j < i) ? "\x1b[93m■\x1b[0m" : "\x1b[90m□\x1b[0m";
        gage += "] \x1b[90m작업 진행 중...\x1b[0m";
        ui->PrintLog(gage, true); 
        Sleep(400);
    }

    ui->ClearLog();
	ui->PrintLog(script.GetFormatStr("ALBA_COMPLETE", {jobName}));
	ui->PrintLog(script.GetFormatStr("ALBA_PAYMENT", {to_string(pay)}));
    // [SOUND] 여기에 알바 정산 효과음을 추가하세요
    Player->AddMoney(pay);
    
    UIManager::WaitKey(ui);
}

void C_Alba::MoveArea()
{
	AS = AlbaState::SelectMenu;
    World->SetHubBG("BG_City");
	World->GotoCity();
}

void C_Alba::Update()
{
    bool isLooping = true;
	while (isLooping)
	{
		switch (AS)
		{
		case AlbaState::SelectMenu:
			SelectMenu();
			break;
		case AlbaState::MoveArea:
			MoveArea();
            isLooping = false;
			break;
		}
	}
}
