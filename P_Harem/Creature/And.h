#pragma once

#include "../Creature/Creature.h"

class And : public C_Creature
{
public:
	And();
    virtual std::string GetImageKey() const override { return "CH_And"; }
};
