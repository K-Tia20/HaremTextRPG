
#include <iostream>
#include "Player.h"
#include "../Creature/Creature.h"
#include "../Inventory/Inventory.h"

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

			i++;
		}

		std::cout << "0. 안싸우기" << std::endl;

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

// 아이템 추가
void C_Player::AddItem(std::shared_ptr<C_Items> item)
{
	Inventory->AddItem(item);
}

// 아이템 사용
std::shared_ptr<C_Items> C_Player::UsingItem()
{
	int randomIndex = rand() % Inventory->GetSize();
	
	Inventory->SubItem(randomIndex);

	return Inventory->GetItem(randomIndex);
}

int C_Player::GetHighlevel()
{
	std::shared_ptr<C_Creature> HighlevelGirl;

	for (auto Girl : GirlFrends)
	{
		if (HighlevelGirl == nullptr)
			HighlevelGirl = Girl;

		if (HighlevelGirl->GetLevel() < Girl->GetLevel())
			HighlevelGirl = Girl;
	}

	return HighlevelGirl->GetLevel();
}
