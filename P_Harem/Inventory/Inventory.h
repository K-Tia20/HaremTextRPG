
#pragma once

#include "../Framework/Framework.h"
#include "../Objects/Items/Item.h"

class C_Items;

class C_Inventory
{
public:
	C_Inventory();

	std::shared_ptr<C_Items> GetItem(int index) { return ItemSlot[index]; }

	int GetSize() { return static_cast<int>(ItemSlot.size()); }
	// 아이템얻기
	void AddItem(std::shared_ptr<C_Items> item);
	// 아이템빼기
	void SubItem(int index);

private:
	std::vector<std::shared_ptr<C_Items>> ItemSlot;
};
