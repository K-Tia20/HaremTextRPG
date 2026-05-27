
#include "Item.h"
#include "../../Framework/Framework.h"
#include "../../Creature/Creature.h"

C_Items::C_Items(std::string name,int price, int value)
{
	SetItem(name, price, value);
}

void C_Items::SetItem(std::string name, int price, int vlaue)
{
	Item = { name, price, 1, vlaue };
}


void C_Items::Add()
{
	Item.Quantity++;
}

void C_Items::Sub()
{
	Item.Quantity--;
}
