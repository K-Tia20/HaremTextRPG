#pragma once

#include "../Creature/Creature.h"

class Zhad : public C_Creature
{
public:
	Zhad();
    virtual std::string GetImageKey() const override { return "CH_Zhad"; }
};

