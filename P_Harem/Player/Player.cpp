
#include <iostream>
#include "Player.h"
#include "../Creature/Creature.h"

C_Player::C_Player()
{
}

std::shared_ptr<C_Creature> C_Player::SetFightGirl()
{
	int choice = 100;

	while (choice > GirlFrends.size())
	{
		std::cout << "싸우게 할 여친을 골라주세요" << std::endl;
		std::cout << std::endl;

		int i = 1;

		for (auto Girls : GirlFrends)
		{
			std::cout << i << ". " << Girls->GetName() << std::endl;
		}

		choice = Input<int>() - 1;
	}

	return GirlFrends[choice];
}


void C_Player::SetName(std::string name)
{
	Name = name;
}

void C_Player::AddGirlFrends(std::shared_ptr<C_Creature> girlfrend)
{
	GirlFrends.push_back(girlfrend);
}

void C_Player::AddMoney(int addMoney)
{
	Money += addMoney;
}

void C_Player::SubMoney(int subMoney)
{
	Money -= subMoney;
}