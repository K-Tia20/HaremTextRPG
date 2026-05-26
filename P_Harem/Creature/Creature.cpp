

#include "Creature.h"

C_Creature::C_Creature(std::string Name, C_Stile Stile, int MaxHp, int Attack)
	: Name(Name), Stile(Stile), MaxHp(MaxHp), CurrentHp(MaxHp), Attack(Attack)
{
	int choice;

	std::cout << "¿î¸íÃ³·³ ¸¸³­ ¼Ò³àÀÇ ÀÌ¸§Àº" << std::endl;
	std::cin >> Name ;
	std::cout << "±×³àÀÇ ¼º°ÝÀº ¾î¶°ÇÑ°¡¿ä? 0)ÇÖ°É 1)Äð³à 2)Äð³à 3)ÀÏ¹Ý³à" << std::endl;
	std::cin >> choice;

	Stile = static_cast<C_Stile>(choice);
}

void C_Creature::TakeDamage(int Damage)
{
	CurrentHp -= Damage;
	if (CurrentHp < 0)
		CurrentHp = 0;
}