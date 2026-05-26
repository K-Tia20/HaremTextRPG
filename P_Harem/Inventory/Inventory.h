
#pragma once

#include "../Framework/Framework.h"
#include "../Objects/Items/Item.h"

class C_Items;

class C_Inventory
{
public:
	C_Inventory();

	int GetItemValue(int index) { return ItemSlot[index]->GetItem().Value; }

	int GetSize() { return ItemSlot.size(); }
	// 아이템얻기
	void AddItem(std::shared_ptr<C_Items> item);
	// 아이템빼기
	void SubItem(int index);

private:
	std::vector<std::shared_ptr<C_Items>> ItemSlot;
};
