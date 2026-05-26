
#include "Inventory.h"
#include "../Objects/Items/Item.h"

C_Inventory::C_Inventory()
{
}

void C_Inventory::AddItem(std::shared_ptr<C_Items> item)
{
	for (std::shared_ptr<C_Items> Item : ItemSlot)
	{
		if (Item->GetItem().Name == item->GetItem().Name)
		{
			Item->Add();
		}
	}

	ItemSlot.push_back(item);
}

void C_Inventory::SubItem(int index)
{
	if (ItemSlot[index]->GetItem().Quantity == 1)
		ItemSlot.erase(ItemSlot.begin() + index);
	else
		ItemSlot[index]->Sub();
}
