#pragma once

#include "../Framework/Framework.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <memory>

class C_Creature;
class C_Items;
class C_Inventory;

class C_Player
{
public:
	C_Player();

    // 입력 인터페이스 고도화
    int InputInt();
    std::string InputString();

	/* 겟터 */
	std::string GetName() { return Name; } 
	std::vector<std::shared_ptr<C_Creature>> GetGirlFrends() { return GirlFrends; } 
	int GetMonny() { return Money; } 

	/* 셋터 */
	void SetName(std::string name); 
	void AddGirlFrends(std::shared_ptr<C_Creature> girlfrend); 
	std::shared_ptr<C_Creature> SetFightGirl(); 
	void AddMoney(int addMoney); 
	void SubMoney(int subMoney); 
	void AddItem(std::shared_ptr<C_Items> item); 
	std::shared_ptr<C_Items> UsingItem(); 
	int GetHighlevel(); 
	void RemoveGirlFriend(std::shared_ptr<C_Creature> girl);

    // 하위 호환성 템플릿 (팀원 코드용)
	template<typename T>
	T Input() {
        if constexpr (std::is_same_v<T, int>) return InputInt();
        else if constexpr (std::is_same_v<T, std::string>) return InputString();
        else { T val; std::cin >> val; return val; }
    }

private:
	std::string Name;
	std::vector<std::shared_ptr<C_Creature>> GirlFrends;
	std::shared_ptr<C_Inventory> Inventory;
	int Money = 0;
};

// 명시적 특수화
template<> inline int C_Player::Input<int>() { return InputInt(); }
template<> inline std::string C_Player::Input<std::string>() { return InputString(); }
