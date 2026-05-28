#pragma once

#include "../Creature/Creature.h"

class Marry : public C_Creature
{
public:
	Marry();
    virtual std::string GetImageKey() const override { return "CH_Marry"; }
};
