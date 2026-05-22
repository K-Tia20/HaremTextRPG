#include "Player.h"
#include "Companion.h"
#include "../Framework/Defs.h" // MAX_EXP, LEVEL_UP_HP_BONUS 등의 상수가 있다고 가정 Text-Console RPG.md]
#include <iostream>

// 초기 스탯 설정 규칙 적용 Text-Console RPG.md]
Player::Player() 
    : name("주인공"), level(1), hp(200), maxHp(200), atk(30), exp(0), gold(0) {
}

Player::~Player() {
    // 동적 할당된 아이템이나 동료가 있다면 여기서 안전하게 메모리를 해제합니다.
    for (auto item : inventory) delete item;
    for (auto companion : haremList) delete companion;
}

// 예외 처리 규칙 적용: 공백 이름 방지 Text-Console RPG.md]
void Player::InputName() {
    while (true) {
        std::cout << "이름을 입력하세요: ";
        std::getline(std::cin, name);

        // 공백 검사 예외 처리 Text-Console RPG.md]
        if (!name.empty() && name.find_first_not_of(' ') != std::string::npos) {
            std::cout << "\n[시스템] 주인공의 이름이 '" << name << "'(으)로 결정되었습니다!\n";
            break;
        }
        std::cout << "[에러] 이름은 공백으로 설정할 수 없습니다. 다시 입력해주세요.\n"; Text-Console RPG.md]
    }
}

// 상시 스탯 확인 기능 구현 Text-Console RPG.md]
void Player::ShowStatus() const {
    std::cout << "\n========================================\n";
    std::cout << "  💖 플레이어 상태 (Lv." << level << " " << name << ") 💖\n"; Text-Console RPG.md]
    std::cout << "========================================\n";
    std::cout << "• HP : " << hp << " / " << maxHp << "\n";
    std::cout << "• ATK : " << atk << "\n";
    std::cout << "• EXP : " << exp << " / 100\n"; Text-Console RPG.md]
    std::cout << "• GOLD : " << gold << " 원\n";
    std::cout << "----------------------------------------\n";
    std::cout << "  👩‍❤️‍👨 보유한 히로인 목록 (총 " << haremList.size() << "명)\n";
    
    if (haremList.empty()) {
        std::cout << "  (아직 영입한 히로인이 없습니다. 나이트클럽으로 가세요!)\n";
    } else {
        for (size_t i = 0; i < haremList.size(); ++i) {
            // 성운 님이 구현할 히로인 이름 출력 메서드 호출 가정
            std::cout << "  [" << i + 1 << "] " << haremList[i]->GetName() << "\n";
        }
    }
    std::cout << "========================================\n";
}

void Player::SetHp(int newHp) {
    hp = newHp;
    if (hp > maxHp) hp = maxHp;
    if (hp < 0) hp = 0;
}

bool Player::SpendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false; // 돈 부족 예외 처리용 신호
}

// 태현 님의 레벨업 시스템과 연동될 로직 Text-Console RPG.md]
void Player::AddExp(int amount) {
    exp += amount;
    std::cout << "[로그] 경험치를 " << amount << " 획득했습니다. (" << exp << "/100)\n"; Text-Console RPG.md]

    if (exp >= 100) { Text-Console RPG.md]
        level++;
        exp -= 100; Text-Console RPG.md]
        
        // 레벨업 시 능력치 상승 규칙 반영 (Lv x 20 체력, Lv x 5 공격력) Text-Console RPG.md]
        maxHp += (level * 20); Text-Console RPG.md]
        atk += (level * 5); Text-Console RPG.md]
        hp = maxHp; // 체력 전원 회복 Text-Console RPG.md]

        std::cout << "\n🎉🎉 LEVEL UP! 플레이어가 레벨 " << level << "이 되었습니다! 🎉🎉\n";
        std::cout << "[성장] 최대 체력이 " << maxHp << "로, 공격력이 " << atk << "로 증가했습니다!\n"; Text-Console RPG.md]
    }
}