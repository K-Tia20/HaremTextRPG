#pragma once

#include "Area.h"
#include "../Framework/Framework.h"

class C_World;
class C_Item;
class C_Player;

enum class ShopState
{
	SelectMenu,
	Purchase,
	Exit
};

class C_Shop : public C_Area
{
public:
	C_Shop(C_World* world);

	virtual void SelectMenu() override;
	virtual void Update() override;

	void Purchase();
	void Exit();

private:
	std::vector<std::shared_ptr<C_Item>> Items;
	std::shared_ptr<C_Player> Player;
	ShopState SS = ShopState::SelectMenu;
};

