
#include "../GameManager/World.h"
#include "../Player/Player.h"
#include "Shop.h"

C_Shop::C_Shop(C_World* world)
{
	World = world;

	Player = World->GetPlayer();
}

void C_Shop::SelectMenu()
{
	int choice = Player->Input<int>();

	switch ()
	{

	}
}

void C_Shop::MoveArea()
{
}

void C_Shop::Update()
{
}

void C_Shop::Encounter()
{
}
