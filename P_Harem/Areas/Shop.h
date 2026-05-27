#pragma once

#include "Area.h"
#include "../Framework/Framework.h"

class C_World;
class C_Items;
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
	virtual void Encounter() {};
	virtual void MoveArea() {};

	void Purchase();
	void Exit();

private:
	std::vector<std::shared_ptr<C_Items>> Items;
	std::shared_ptr<C_Player> Player;
	ShopState SS = ShopState::SelectMenu;
};

