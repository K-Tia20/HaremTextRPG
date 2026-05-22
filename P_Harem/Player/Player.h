
#pragma once

#include "../Framework/Framework.h"

class C_Creature;
class C_Item;
class C_Inventory;

class C_Player
{
public:
	C_Player();

	template<typename T>
	T Input()
	{
		T value;
		
		cin >> value;

		return value;
	}

	template<>
	int Input<int>()
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

	// 겟터
	std::string GetName() const { return Name; }
	int GetMonny() const { return monny; }
	std::vector<std::shared_ptr<C_Creature>> GetGirlFrends() const { return GirlFrends; }

	// 셋터
	void SetName(std::string name);

private:
	std::string Name;
	std::vector<std::shared_ptr<C_Creature>> GirlFrends;
	std::shared_ptr<C_Inventory> Inventory;
	int monny = 0;
};