#pragma once

class C_Player;
class C_Area;

// 현재 World의 게임진행 상황
enum class WorldState
{
	NewGame,
	StartGame,
	InProgress,
	QuitGame
};

// Player의 위치
enum class WorldArea
{
	City,
	Club,
	Store
};

class C_World
{
public: // 게임 진행 함수
	C_World();

	// 새 게임 진행
	void NewGame();
	// 게임 시작 부분 (이부분의 끝나면 WS를 InProgress로 변경)
	void StartGame();
	// 게임진행 중
	void InProgress();

	// Tick처럼 지속적으로 업데이트 
	void Update();

public: // 게임 진행 로직들

	void GotoCity() { CL = WorldArea::City; }

	void GotoShop() { CL = WorldArea::Store; }

	// 이름 설정 로직
	void SetName();

	// 첫 여자친구 설정 로직
	void SetGirlFrends();

	/* TODO : 기본여친 클래스 만들어지면 전부 변경 */
	// 테토걸로 설정
	void SetTetoGirl();

	// 쿨미녀걸로 설정
	void SetCoolPretyGirl();

	// 초식녀로 설정
	void SetChosicGirl();
	/* -------------------------------------- */

	// 플레이어 및 히로인 설정 확인 후 
	bool CheckInit();

public: // 겟터
	std::shared_ptr<C_Player>GetPlayer() { return Player; }

private:
	// 변수명 편하게 바꿔주세요

	// 플레이어
	std::shared_ptr<C_Player> Player;

	// 현재 위치
	std::shared_ptr<C_Area> CurrentArea;

	// World가 알고 있는 위치정도 찾아서 설정해주기 위한 용도
	std::unordered_map<WorldArea, std::shared_ptr<C_Area>> Areas;

	// World의 게임상태
	WorldState WS = WorldState::NewGame;

	// CurrentLocatoin이라는 뜻
	WorldArea CL;

	// 게임종료 로직에 사용될 변수
	bool IsRunning;
};