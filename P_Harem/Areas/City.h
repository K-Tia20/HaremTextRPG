#pragma once

#include "Area.h"

class C_City : public C_Area
{
public:
	//C_City(C_World* world);
	C_City();

	virtual void SelectMenu() override;
	virtual void MoveArea() override;
	virtual void Update() override;
	virtual void Encounter() override;
};

