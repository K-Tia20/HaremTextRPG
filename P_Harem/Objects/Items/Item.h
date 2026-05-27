#pragma once

#include <string>

enum class ItemType
{
	Heal,
	Power,
	Defence
};

// 아이템 정보
struct ItemDatas
{
	std::string Name;
	int Price;
	int Quantity = 1;
	int Value;
	ItemType Type;
};

class C_Creature;

class C_Items
{
public:
	C_Items(std::string name, int price, int value, ItemType type);

	// 겟터
	const ItemDatas& GetItem() const { return Item; }

	// 셋터
	void SetItem(std::string name, int price, int value, ItemType type);
	void Add();
	void Sub();

	// 아이템 사용시 아이템 종류에 따라 아이템 로직이 달라짐
	int UsingItem() { return Item.Value; }

private:
	ItemDatas Item;
};