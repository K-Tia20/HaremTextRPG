#pragma once

#include <string>

// 아이템 정보
struct ItemDatas
{
	std::string Name;
	int Price;
	int Quantity = 1;
	int Value;
};

class C_Creature;

class C_Items
{
public:
	C_Items(std::string name, int price, int value);

	// 겟터
	ItemDatas GetItem() const { return Item; }

	// 셋터
	void SetItem(std::string name, int price, int Value);
	void Add();
	void Sub();

	// 아이템 사용시 아이템 종류에 따라 아이템 로직이 달라짐
	int UsingItem() { return Item.Value; }

private:
	ItemDatas Item;
};