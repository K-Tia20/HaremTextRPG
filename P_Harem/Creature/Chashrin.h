#pragma once

#include "../Creature/Creature.h"

class Chashrin : public C_Creature
{
public:
	Chashrin();
    virtual std::string GetImageKey() const override { return "CH_Chashrin"; }
};
