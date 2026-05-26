#pragma once

#include "Area.h"
#include "../Framework/Framework.h"

class C_World;
class C_Item;

class C_Shop : public C_Area
{
public:
	C_Shop(C_World* world);

	virtual void SelectMenu() override;
	virtual void MoveArea() override;
	virtual void Update() override;
	virtual void Encounter() override;

private:
	std::vector<std::shared_ptr<C_Item>> Items;
};

