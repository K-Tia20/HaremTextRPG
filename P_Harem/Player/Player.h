
#pragma once

#include "../Framework/Framework.h"

class C_Creature;
class C_Items;
class C_Inventory;

class C_Player
{
public:
	C_Player();


	template<typename T>
	static T Input()
	{
		T value;
		
		std::cin >> value;

		return value;
	}

	template<>
	static int Input<int>()
	{
		int value;

		while (true)
		{
			if (std::cin >> value)
			{
				if (std::cin.peek() == '\n')
				{
					return value;
				}
			}

			std::cout << "정수만 입력해주세요." << std::endl;

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	/* 겟터 */
	std::string GetName() { return Name; } // 플레이어 이름
	std::vector<std::shared_ptr<C_Creature>>GetGirlFrends() { return GirlFrends; } // 가진 여자친구
	int GetMonny() { return Money; } // 플레이어가 가진 돈

	/* 셋터 */
	void SetName(std::string name); // 플레이어 이름 정하기
	void AddGirlFrends(std::shared_ptr<C_Creature> girlfrend); // 여자친구 얻기
	std::shared_ptr<C_Creature> SetFightGirl(); // 이걸로 전투할 여친 고르기
	void AddMoney(int addMoney); // 돈먹기
	void SubMoney(int subMoney); // 돈빼앗기기
	void AddItem(std::shared_ptr<C_Items> item); // 아이템먹기
	int UsingItem(); // 아이템빼앗기기 (사용이라는 뜻)
	int GetHighlevel(); // 가장 높은 여친의 레벨받아오기

private:
	std::string Name;
	std::vector<std::shared_ptr<C_Creature>> GirlFrends;
	std::shared_ptr<C_Inventory> Inventory;
	int Money = 0;
};