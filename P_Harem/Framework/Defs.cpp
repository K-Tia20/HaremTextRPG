#include "Defs.h"

int Creature::GetRequiredAffinity()
{
	if (Level >= 10)
		return 0;

	return RequiredAffinity[Level - 1];
}
void Creature::GainAffinity(int Affinity)
{
	if (Level >= 10)
		return;

	CurrentAffinity += Affinity;
	CheckLevelUp();
}
void Creature::CheckLevelUp()
{
	while (Level < 10 && CurrentAffinity >= GetRequiredAffinity())
	{
		CurrentAffinity -= GetRequiredAffinity();
		LevelUp();
	}
}
void Creature::LevelUp()
{
	MaxHp += Level * 20;
	Attack += Level * 5;
	Level++;
	Hp = MaxHp;
}