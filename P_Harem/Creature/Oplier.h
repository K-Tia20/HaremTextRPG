#pragma once

#include "../Creature/Creature.h"

class Oplier : public C_Creature
{
public:
	Oplier();
    virtual std::string GetImageKey() const override { return "CH_Oplier"; }
};


