// Creature.cpp

#include "Creature.h"

C_Creature::C_Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack)
	: Name(Name), Stile(Stile), MaxHp(MaxHp), CurrentHp(MaxHp), Attack(Attack)
{

}

void C_Creature::TakeDamage(int Damage)
{
	CurrentHp -= Damage;
	if (CurrentHp < 0)
		CurrentHp = 0;
}