#include "IUnit.h"

IUnit::IUnit(std::string _name, int _team)
{
	name = _name;
	hpState = IUnit::unitHealthState::alive;
	team = _team;
}

IUnit::~IUnit()
{
}

void IUnit::takeDamage(int amount) {
	if (health > 0) {
		health -= amount;
		if (health <= 0) {
			hpState = IUnit::unitHealthState::dead;
		}
	}
}