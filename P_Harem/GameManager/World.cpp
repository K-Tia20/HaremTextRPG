#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "../Areas/Area.h"
#include "../Areas/City.h"
#include "../Areas/Shop.h"
#include "../Areas/Alba.h"
#include "../Creature/Creature.h"
#include "../UI/UIManager.h"
#include "../UI/ImageManager.h"
#include "../UI/ScriptManager.h"
#include "../UI/DelegateManager.h"
#include "../Battle/BattleSystem.h"
#include "../UI/LogSystem.h"
#include "../UI/CSoundManager.h"//추가햇어


using namespace std;

static C_World* g_WorldInstance = nullptr;

C_World& C_World::GetInstance() {
    return *g_WorldInstance;
}

C_World::C_World()
{
    g_WorldInstance = this; 
	Player = make_shared<C_Player>();
    m_ui = make_unique<UIManager>();
    m_battle = make_unique<C_BattleSystem>(this);

	Areas[WorldArea::City] = make_shared<C_City>(this);
	Areas[WorldArea::Store] = make_shared<C_Shop>(this); 
    Areas[WorldArea::Alba] = make_shared<C_Alba>(this);
}

C_World::~C_World() {
    if (g_WorldInstance == this) g_WorldInstance = nullptr;
}

/**
 * [Init] - 시네마틱 오프닝 및 엔진 초기 가동 (최정예 연출 버전)
 */
void C_World::Init() {
    C_ImageManager::GetInstance().Init();
    C_ScriptManager::GetInstance().Init();
    m_ui->Init();
    
    auto& script = C_ScriptManager::GetInstance();
    auto& img = C_ImageManager::GetInstance();

    //[음악시작 : 오프닝]
    auto& sound = CSoundManager::GetInstance();
    sound.PlayBGM(L"../P_Harem/Sound/BGM/Opening.wav");
    
    // 1. [프롤로그] 운명적인 첫 문구
    m_ui->CenteredTypeLog(script.Get("SCENE_OPENING_1"), 22, 1000);
    Sleep(2000);
    m_ui->PlayBlueTransition(); // 화면 정화
    
    
    // 2. [팀 로고] BG_TeamName.png 출력
    // (뷰포트가 아닌 전체 화면 중앙 느낌을 위해 DrawImage를 일시 활용)
    m_ui->DrawImage(img.GetLayeredImage("BG_TeamName", {})); 
    Sleep(3000);
    m_ui->PlayBlueTransition();

    // 3. [게임 타이틀] BG_Title.png 출력
    m_ui->DrawImage(img.GetLayeredImage("BG_Title", {}));
    Sleep(3000);

    //[음악종료 : 오프닝]
    sound.StopBGM();
    
    //[음악시작 : 첫만남]
    sound.PlayBGM(L"../P_Harem/Sound/BGM/MainMenu.wav");

  
    // 4. [UI 조립] 테두리가 보이면서 뷰포트에 타이틀 안착
    m_ui->RenderMainUI();
    m_ui->UpdateDate(GetCurrentDateString());
    m_ui->ClearMainViewport();
    m_ui->DrawImage(img.GetLayeredImage("BG_Title", {}));
    
    m_ui->PrintLog(script.Get("TITLE_MAIN"));
    UIManager::WaitKey(m_ui.get());

    DelegateManager dm;
    dm.BindAll(m_ui.get(), m_battle.get());
    
}

void C_World::Update()
{
    if (m_isFirstFrame) {
        Init();
        m_isFirstFrame = false;
        return;
    }

    // if (Player->GetGirlFrends().empty()) SoloEnd();
    
    if (!IsRunning) return;

    SyncUI();
    
    auto& script = C_ScriptManager::GetInstance();
    
	switch (WS)
	{
	case WorldState::NewGame:    NewGame(); break;
	case WorldState::StartGame:  StartGame(); break;
	case WorldState::InProgress: 
    {
        m_ui->ClearMainViewport();
        m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage(m_hubBG, {}));
        m_ui->PrintLog(script.Get("MAIN_MENU"));
        
        int choice = Player->InputInt();
        m_ui->ClearLog();

        bool goToArea = false;
        switch (choice) {
            case 1: Areas[WorldArea::Store]->Enter(); GotoShop(); goToArea = true; break;
            case 2: Areas[WorldArea::City]->Enter(); GotoCity(); goToArea = true; break; 
            case 3: Areas[WorldArea::Alba]->Enter(); GotoAlba(); goToArea = true; break;
            case 4: C_LogSystem::GetInstance().ShowContactList(); break;
            case 5: C_LogSystem::GetInstance().ShowInventory(); break;
            case 0: WS = WorldState::QuitGame; break;
            default: m_ui->PrintLog("시스템: 잘못된 선택입니다."); UIManager::WaitKey(m_ui.get()); break;
        }

        if (goToArea && WS != WorldState::QuitGame) {
            InProgress(); 
            CL = WorldArea::City; 
        }
        break;
    }
	case WorldState::QuitGame:   IsRunning = false; break;
	default: WS = WorldState::QuitGame; break;
	}
}

void C_World::Run() {
    while (IsRunning) {
        Update();
        Sleep(50);
    }
}

void C_World::NewGame()
{
	if (Player->GetName().empty()) SetName();
	else if (Player->GetGirlFrends().empty()) SetGirlFrends();
    else WS = WorldState::StartGame;
}

void C_World::StartGame()
{ 
    CL = WorldArea::City;
    WS = WorldState::InProgress;
}

void C_World::InProgress()
{
	Areas[CL]->Update();
}

string C_World::GetCurrentDateString() const {
    return to_string(m_month) + "월 " + to_string(m_day) + "일";
}

void C_World::SyncUI() {
    if (!Player || !m_ui) return;
    
    bool needsUpdate = false;
    if (m_lastName != Player->GetName()) { m_lastName = Player->GetName(); m_ui->UpdateUserInfo(m_lastName); }
    if (m_lastMoney != Player->GetMonny()) { m_lastMoney = Player->GetMonny(); m_ui->UpdateMoney(m_lastMoney); }
    string currentDate = GetCurrentDateString();
    if (m_lastDate != currentDate) { m_lastDate = currentDate; m_ui->UpdateDate(m_lastDate); }

    const auto& friends = Player->GetGirlFrends();
    if (m_lastHeroineCount != friends.size()) { needsUpdate = true; m_lastHeroineCount = friends.size(); }
    else {
        static std::vector<int> lastHPs, lastLvs, lastAfs;
        if (lastHPs.size() != friends.size()) {
            lastHPs.resize(friends.size()); lastLvs.resize(friends.size()); lastAfs.resize(friends.size());
            needsUpdate = true;
        }
        for (size_t i = 0; i < friends.size(); ++i) {
            if (lastHPs[i] != friends[i]->GetCurrentHp() || lastLvs[i] != friends[i]->GetLevel() || lastAfs[i] != friends[i]->GetAffinity()) {
                lastHPs[i] = friends[i]->GetCurrentHp(); lastLvs[i] = friends[i]->GetLevel(); lastAfs[i] = friends[i]->GetAffinity();
                needsUpdate = true;
            }
        }
    }

    if (needsUpdate) {
        vector<UIManager::HeroineDisplayData> displayList;
        for (const auto& f : friends) {
            std::string sStyle = "평범녀";
            if (f->GetStile() == C_Stile::HotGirl) sStyle = "핫걸";
            else if (f->GetStile() == C_Stile::IceGirl) sStyle = "냉미녀";
            else if (f->GetStile() == C_Stile::GrassGirl) sStyle = "초식녀";
            displayList.push_back({f->GetName(), f->GetLevel(), f->GetCurrentHp(), f->GetMaxHp(), f->GetAttack(), f->GetAffinity(), sStyle});
        }
        m_ui->UpdateHeroineList(displayList);
    }
}

void C_World::SetName()
{
    auto& script = C_ScriptManager::GetInstance();
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Title", {})); 

    m_ui->ClearLog();
	m_ui->PrintLog(script.Get("INPUT_PLAYER_NAME"));
	string newname = Player->InputString(); 

    m_ui->ClearLog();
	m_ui->PrintLog("시스템: [" + newname + "] 님으로 등록되었습니다.");
    Player->SetName(newname);
    SyncUI();
}

void C_World::SetGirlFrends()
{
    auto& script = C_ScriptManager::GetInstance();
    auto& img = C_ImageManager::GetInstance();

    m_ui->ClearMainViewport();
    // [Visual Upgrade] 속성을 모르는 상태이므로 CH_Null 실루엣 출력
    m_ui->DrawImage(img.GetLayeredImage("BG_City2", {{"CH_Null", 50, 0, false}})); 

	  m_ui->PrintLog(script.Get("SCENE_OPENING_2")); 
    UIManager::WaitKey(m_ui.get());

	  m_ui->PrintLog(script.Get("INPUT_HEROINE_NAME"));
	  string girlName = SetGirlFrendName();
    m_ui->ClearLog();
  
    //[음악설정뭔가]
    auto& sound = CSoundManager::GetInstance();
    //[음악종료 : 첫만남]
    sound.StopBGM();
    
    //[음악시작 : 선택 첫여친]
    sound.PlayBGM(L"../P_Harem/Sound/BGM/Firstcoffee.wav");
    
    bool isValidChoice = false;
  
    // 캐릭터 선택 시 1~4번만 선택 가능하도록 예외 처리 추가
    do
    {
        m_ui->PrintLog(script.Get("SELECT_HEROINE_PERSONALITY"));
        int choice = Player->InputInt();
        m_ui->ClearLog();

        switch (choice)
        {
        case 1: 
            SetTetoGirl(girlName); 
            isValidChoice = true; 
            break;
        case 2: 
            SetChosicGirl(girlName); 
            isValidChoice = true; 
            break;
        case 3: 
            SetCoolPretyGirl(girlName); 
            isValidChoice = true; 
            break;
        case 4: 
            SetNormalGirl(girlName); 
            isValidChoice = true; 
            break;
        default: 
            m_ui->PrintLog(script.Get("FAILED_SELECT_HEROINE_PERSONALITY"));
            break;
        }
    }
    while (!isValidChoice);

	if (!Player->GetGirlFrends().empty())
	{
        // [사용자 요청] 데이트 연출 추가
        m_ui->ClearMainViewport();
        m_ui->DrawImage(img.GetLayeredImage("BG_YogerPresso_I", {})); 
        m_ui->PrintLog("시스템: 그녀와 요거플레쏘에서 즐거운 시간을 보낸 뒤, 전화번호를 교환했습니다.");
        UIManager::WaitKey(m_ui.get());
        
        // [사용자 요청] 엔터 누른 후 방으로 복귀
        m_ui->ClearMainViewport();
        m_ui->DrawImage(img.GetLayeredImage("BG_Room", {})); 
	    
	    //[음악종료 : 선택 첫여친]
	    sound.StopBGM();
	}
}

void C_World::SetTetoGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::HotGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City2", {{"CH_Red", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_HOT", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetCoolPretyGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::IceGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City2", {{"CH_Blue", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_COOL", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetChosicGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::GrassGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City2", {{"CH_Green", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_HERB", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetNormalGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::NormalGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City2", {{"CH_Normal", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_NORMAL", {name}));
    UIManager::WaitKey(m_ui.get());
}

// 여기는 
void C_World::SoloEnd()
{
    IsRunning = false;
}

// 여기서 이김 엔딩
void C_World::RealEnd()
{
    IsRunning = false;
}

// 여기서 배드엔딩
void C_World::BadEnd()
{
    IsRunning = false;
}

UIManager* C_World::GetUI() { return m_ui.get(); }
C_BattleSystem* C_World::GetBattle() { return m_battle.get(); }
string C_World::SetGirlFrendName() { return Player->InputString(); }

