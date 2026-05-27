#include "../Framework/Framework.h"
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
#include "World.h"

using namespace std;

// [공존형 설계] 전역 인스턴스를 관리하여 싱글톤과 shared_ptr 시스템을 연결합니다.
static C_World* g_WorldInstance = nullptr;

C_World& C_World::GetInstance() {
    return *g_WorldInstance;
}

C_World::C_World()
{
    g_WorldInstance = this; // 생성 시점에 전역 주소 등록
	Player = make_shared<C_Player>();
    m_ui = make_unique<UIManager>();
    m_battle = make_unique<C_BattleSystem>(this); // 배선 연결

	Areas[WorldArea::City] = make_shared<C_City>(this);
	Areas[WorldArea::Store] = make_shared<C_Shop>(this);
    Areas[WorldArea::Alba] = make_shared<C_Alba>(this);
}

C_World::~C_World() {
    if (g_WorldInstance == this) g_WorldInstance = nullptr;
}

/**
 * [Init] - 팀원들의 로직 가동 전, 화려한 무대를 준비합니다.
 */
void C_World::Init() {
    C_ImageManager::GetInstance().Init();
    C_ScriptManager::GetInstance().Init();
    m_ui->Init();
    
    // 시네마틱 오프닝 연출
    m_ui->CenteredTypeLog("소년이여, 신화가 되라.", 22);
    Sleep(2000);
    m_ui->PlayBlueTransition();

    m_ui->RenderMainUI();
    m_ui->UpdateDate(GetCurrentDateString());

    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Title", {}));
    
    auto& script = C_ScriptManager::GetInstance();
    m_ui->PrintLog(script.Get("TITLE_MAIN"));
    UIManager::WaitKey(m_ui.get());

    // 로직과 UI의 가교를 놓습니다. (팀의 로직을 UI에 연결)
    DelegateManager dm;
    dm.BindAll(m_ui.get(), m_battle.get());
}

/**
 * [Update] - 팀원들이 설계한 메인 게임 루프
 */
void C_World::Update()
{
    if (m_isFirstFrame) {
        Init();
        m_isFirstFrame = false;
        return;
    }

    if (!IsRunning) return;

    SyncUI(); // 팀의 데이터(돈, HP)를 UI에 실시간 반영

    auto& script = C_ScriptManager::GetInstance();

	switch (WS)
	{
	case WorldState::NewGame:    NewGame(); break;
	case WorldState::StartGame:  StartGame(); break;
	case WorldState::InProgress: 
    {
        // [거점 메뉴 연출] 항상 내 방 배경과 메뉴를 출력
        m_ui->ClearMainViewport();
        m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Room", {}));
        m_ui->PrintLog(script.Get("MAIN_MENU"));
        
        int choice = Player->InputInt();
        bool goToArea = false;

        switch (choice) {
            case 1: GotoShop(); goToArea = true; break;
            case 2: GotoCity(); goToArea = true; break; 
            case 3: GotoAlba(); goToArea = true; break;
            case 4: C_LogSystem::GetInstance().ShowContactList(); break;
            case 5: C_LogSystem::GetInstance().ShowInventory(); break;
            case 6: WS = WorldState::QuitGame; break;
            default: m_ui->PrintLog("시스템: 잘못된 선택입니다."); UIManager::WaitKey(m_ui.get()); break;
        }

        if (goToArea && WS != WorldState::QuitGame) {
            InProgress(); // 해당 구역 업데이트 루프 진입
            CL = WorldArea::City; // 구역에서 나오면 거점으로 복귀
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
    // 팀원들의 NewGame 설계 순서를 따르되 UI 연출 적용
	if (Player->GetName().empty()) SetName();
	if (!Player->GetName().empty() && Player->GetGirlFrends().empty()) SetGirlFrends();
    if (!Player->GetName().empty() && !Player->GetGirlFrends().empty()) WS = WorldState::StartGame;
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
    if (m_lastName != Player->GetName()) {
        m_lastName = Player->GetName();
        m_ui->UpdateUserInfo(m_lastName);
    }
    if (m_lastMoney != Player->GetMonny()) {
        m_lastMoney = Player->GetMonny();
        m_ui->UpdateMoney(m_lastMoney);
    }
    string currentDate = GetCurrentDateString();
    if (m_lastDate != currentDate) {
        m_lastDate = currentDate;
        m_ui->UpdateDate(m_lastDate);
    }

    const auto& friends = Player->GetGirlFrends();
    if (m_lastHeroineCount != friends.size()) {
        needsUpdate = true;
        m_lastHeroineCount = friends.size();
    } else {
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
    m_ui->PrintLog(script.Get("SCENE_OPENING_2"));
    UIManager::WaitKey(m_ui.get());

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
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City", {})); 

	m_ui->PrintLog(script.Get("INPUT_HEROINE_NAME"));
	string girlName = SetGirlFrendName();
    m_ui->ClearLog();

	m_ui->PrintLog(script.Get("SELECT_HEROINE_PERSONALITY"));
	int choice = Player->InputInt();
    m_ui->ClearLog();

	switch (choice)
	{
	case 1: SetTetoGirl(girlName); break;
	case 2: SetChosicGirl(girlName); break;
	case 3: SetCoolPretyGirl(girlName); break;
	default: SetNormalGirl(girlName); break;
	}

	if (!Player->GetGirlFrends().empty())
	{
        m_ui->ClearMainViewport();
        m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_Room", {})); 
        m_ui->PrintLog("시스템: 그녀와의 인연을 뒤로하고 집으로 돌아왔습니다.");
        UIManager::WaitKey(m_ui.get());
	}
}

void C_World::SetTetoGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::HotGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City", {{"CH_Red", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_HOT", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetCoolPretyGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::IceGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City", {{"CH_Blue", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_COOL", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetChosicGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::GrassGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City", {{"CH_Green", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_HERB", {name}));
    UIManager::WaitKey(m_ui.get());
}
void C_World::SetNormalGirl(string name) {
    auto& script = C_ScriptManager::GetInstance();
    auto girl = make_shared<C_Creature>(name, C_Stile::NormalGirl, 200, 30);
    Player->AddGirlFrends(girl);
    m_ui->ClearMainViewport();
    m_ui->DrawImage(C_ImageManager::GetInstance().GetLayeredImage("BG_City", {{"CH_Normal", 50, 0}}));
    m_ui->TypeLog(script.GetFormatStr("INTRO_HEROINE_NORMAL", {name}));
    UIManager::WaitKey(m_ui.get());
}

UIManager* C_World::GetUI() { return m_ui.get(); }
C_BattleSystem* C_World::GetBattle() { return m_battle.get(); }
string C_World::SetGirlFrendName() { return Player->InputString(); }
bool C_World::CheckInit() { return false; }
