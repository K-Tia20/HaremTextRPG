

#include "Creature.h"

C_Creature::C_Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack)
	: Name(Name), Stile(Stile), MaxHp(MaxHp), CurrentHp(MaxHp), Attack(Attack)
{
	Affinity = 0;
}

void C_Creature::TakeDamage(int Damage)
{
	CurrentHp -= Damage;
	if (CurrentHp < 0)
		CurrentHp = 0;
}


void C_Creature::SetEnemyStat(int level)
{
	Level = level;

	MaxHp =
		(rand() % (level * 10 + 1))
		+ (level * 20);

	Attack =
		(rand() % (level * 5 + 1))
		+ (level * 5);

	CurrentHp = MaxHp;
}
