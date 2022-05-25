#include "Scout.h"

Scout::Scout(std::string _name, int _team) : ITroop(_name, _team)
{
	health = hp;
	speed = mS;

	attackSpeed = aS;
	damage = d;
	range = r;
}

Scout::~Scout()
{
}

void Scout::onInit() {
	std::cout << "New " << name << " produced" << '\n';
}

void Scout::onHit() {
	
}

void Scout::onDeath() {
	std::cout << name << " died" << '\n';
}