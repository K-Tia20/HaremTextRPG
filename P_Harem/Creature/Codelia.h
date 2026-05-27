#pragma once

#include "../Creature/Creature.h"

class Codelia : public C_Creature
{
public:
	Codelia();
    virtual std::string GetImageKey() const override { return "CH_Codelia"; }
};

