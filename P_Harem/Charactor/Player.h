#pragma once
#include "../Framework/Framework.h"  // STL 및 공용 include가 들어있는 헤더
#include "../Framework/Enums.h"      // Attribute 열거형 포함
#include "../Objects/Item.h"         // 승종 님의 아이템 클래스 참조

// Companion 클래스의 전방 선언 (순환 참조 방지)
class Companion;

class Player {
private:
    std::string name;                // 플레이어 닉네임 Text-Console RPG.md]
    int level;                       // 레벨 (초기값 1) Text-Console RPG.md]
    int hp;                          // 현재 체력 Text-Console RPG.md]
    int maxHp;                       // 최대 체력 (초기값 200) Text-Console RPG.md]
    int atk;                         // 공격력 (초기값 30) Text-Console RPG.md]
    int exp;                         // 현재 경험치 (초기값 0) Text-Console RPG.md]
    int gold;                        // 소지 골드

    std::vector<Item*> inventory;       // 승종 님 담당: 소지 아이템 가방
    std::vector<Companion*> haremList;  // 성운 님 담당: 영입한 동료(히로인) 목록

public:
    Player();
    ~Player();

    // --- 필수 및 기획 동작 관련 메서드 ---
    void InputName();                // 플레이어 닉네임 결정 Text-Console RPG.md]
    void ShowStatus() const;         // 상시 캐릭터 상태 및 동료 확인 기능 Text-Console RPG.md]
    
    // --- 전투 및 아이템 상호작용 (윤택, 승종 님 연동) ---
    void UseItem(int index);         // 인벤토리의 아이템 사용
    void AddItem(Item* item);        // 상점 구매 및 전투 보상 시 아이템 추가
    void AddCompanion(Companion* companion); // 새로운 히로인 겟또다제! (동료 추가)

    // --- 성장 및 재화 관리 (태현, 승종 님 연동) ---
    void AddGold(int amount) { gold += amount; }
    bool SpendGold(int amount);      // 상점 구매 가능 여부 체크 및 차감
    void AddExp(int amount);         // 경험치 획득 및 레벨업 판단 Text-Console RPG.md]

    // --- 윤택 님 전투 시스템용 Getters ---
    std::string GetName() const { return name; }
    int GetHp() const { return hp; }
    void SetHp(int newHp);
    int GetAtk() const { return atk; }
    std::vector<Companion*>& GetHaremList() { return haremList; }
};