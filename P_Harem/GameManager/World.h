#pragma once
#include "../Framework/Framework.h"
#include <memory>
#include <unordered_map>
#include <string>

class C_Player;
class C_Area;
class UIManager;
class C_BattleSystem;

// 현재 World의 게임진행 상황
enum class WorldState { NewGame, StartGame, InProgress, QuitGame };
enum class WorldArea { City, Store, Alba };

class C_World : public std::enable_shared_from_this<C_World>
{
public: 
    // [공존형 설계] 싱글톤 접근과 명시적 생성을 모두 지원합니다.
    static C_World& GetInstance(); 

	C_World();
    ~C_World();

	void Init(); 
    void Run();  
	void NewGame();
	void StartGame();
	void InProgress();
	void Update(); 

    void SyncUI();
    std::string GetCurrentDateString() const;
    
    UIManager* GetUI();
    C_BattleSystem* GetBattle();
	std::shared_ptr<C_Player> GetPlayer() { return Player; }
    
    // 동적 거점 배경 제어
    void SetHubBG(const std::string& bgName) { m_hubBG = bgName; }
	void AdvanceDay();
	// 날짜 기능 추가합니다 -똥쟁이 우현

public: // 지역 이동 및 셋팅 로직 (팀의 기존 멤버 보존)
	void GotoCity() { CL = WorldArea::City; }
	void GotoShop() { CL = WorldArea::Store; }
	void GotoAlba() { CL = WorldArea::Alba; }

	void SetName();
	void SetGirlFrends();

	void SetTetoGirl(std::string name);
	void SetCoolPretyGirl(std::string name);
	void SetChosicGirl(std::string name);
	void SetNormalGirl(std::string name);
	
	// 여친전부 방출엔딩
	void SoloEnd();
	// 보스 잡음
	void RealEnd();
	// 보스한테 죽음
	void BadEnd();
	std::string SetGirlFrendName();

private:
	std::shared_ptr<C_Player> Player;
	std::shared_ptr<C_Area> CurrentArea;
	std::unordered_map<WorldArea, std::shared_ptr<C_Area>> Areas;

	WorldState WS = WorldState::NewGame;
	WorldArea CL = WorldArea::City;
	bool IsRunning = true;

    // UI 엔진 서브시스템
    std::unique_ptr<UIManager> m_ui;
    std::unique_ptr<C_BattleSystem> m_battle;

    // 더티 플래그 캐시 (실시간 동기화용)
    std::string m_lastName = "";
    int m_lastMoney = -1;
    std::string m_lastDate = "";
    size_t m_lastHeroineCount = 0;

    int m_month = 3;
    int m_day = 1;
    bool m_isFirstFrame = true;
    std::string m_hubBG = "BG_Room"; // 초기 거점은 내 방
};
