#include "Barracks.h"

Barracks::Barracks(std::string _name, int _team, int _type) : Building(_name, _team, _type)
{
	health = hp;
	buildTime = bT;
	cost = c;

	onInit();
}

Barracks::~Barracks()
{
}

void Barracks::onInit()
{
}

void Barracks::onHit()
{
}

void Barracks::onDeath()
{
	std::cout << name << " Destroyed\n";
}

