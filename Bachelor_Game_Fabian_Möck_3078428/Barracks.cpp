#include "Barracks.h"

Barracks::Barracks(std::string _name, int _team) : Building(_name, _team)
{
	health = hp;
	buildTime = bT;
	//buildOptions = bO;

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

