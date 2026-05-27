#pragma once

#include "Area.h"

enum class AlbaState
{
	SelectMenu,
	MoveArea,
	Alba
};

class C_Player;

class C_Alba : public C_Area
{
public:
	C_Alba(C_World* world);

	virtual void SelectMenu() override;
	virtual void Update() override;
	virtual void MoveArea() override;
	virtual void Encounter() {};

private:
	std::shared_ptr<C_Player> Player;
	AlbaState AS = AlbaState::SelectMenu;
};