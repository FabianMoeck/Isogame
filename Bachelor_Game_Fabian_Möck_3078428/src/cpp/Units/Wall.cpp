#include "../../Headers/Units/Wall.h"

Wall::Wall(std::string _name, int _team, int _type) : Building(_name, _team, _type)
{
	health = hp;
	buildTime = bT;
	cost = c;

	onInit();
}

Wall::~Wall()
{
}

void Wall::onInit()
{
	//std::cout << "new Wall\n";
}

void Wall::onHit()
{
}

void Wall::onDeath()
{
	std::cout << name << " Destroyed\n";
}