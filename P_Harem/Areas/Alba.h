#pragma once

#include "Area.h"

class C_Player;

class C_Alba : public C_Area
{
public:
	C_Alba(C_World* world);

	virtual void SelectMenu() override;
	virtual void MoveArea() override;
	virtual void Update() override;


private:
	std::shared_ptr<C_Player> Player;
};