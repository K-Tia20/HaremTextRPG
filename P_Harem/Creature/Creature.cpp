// Creature.cpp

#include "Creature.h"

Creature::Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack, int Affinity)
	: Name(Name), Stile(Stile), MaxHp(MaxHp), CurrentHp(MaxHp), Attack(Attack), Affinity(Affinity)
{

}

void Creature::TakeDamage(int Damage)
{
	CurrentHp -= Damage;
	if (CurrentHp < 0)
		CurrentHp = 0;
}