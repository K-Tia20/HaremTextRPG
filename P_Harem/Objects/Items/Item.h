#pragma once

#include <string>


enum class ItemType
{
	HealPotion,
	PowerUpPotion,
	DefenceUpPotion
};

// 아이템 정보
struct ItemDatas
{
	std::string Name;
	ItemType Itemtype;
	int Price;
	int Quantity;
	int Value;
};

class C_Items
{
public:
	C_Items();


	// 겟터
	ItemDatas GetItem() const { return Item; }

	// 셋터
	void SetItemName(std::string itemname);
	void SetPrice(int price);

	// 아이템 사용시 아이템 종류에 따라 아이템 로직이 달라짐
	void UsingItem();

	// 아이템 로직
	void UsingHpItem(/*Creature 변수 사용해야함*/);
	void UsingPowerItem(/*Creature 변수 사용해야함*/);
	void UsingDefenceItem(/*Creature 변수 사용해야함*/);

private:
	ItemDatas Item;
};

