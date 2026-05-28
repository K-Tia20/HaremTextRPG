#pragma once

#include "../Creature/Creature.h"

class Lina : public C_Creature
{
public:
	Lina();
    virtual std::string GetImageKey() const override { return "CH_Lina"; }
};

