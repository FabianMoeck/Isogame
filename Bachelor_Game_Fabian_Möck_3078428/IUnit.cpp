#include "IUnit.h"

IUnit::IUnit(std::string _name, int _team, int _type)
{
	name = _name;
	hpState = IUnit::unitHealthState::alive;
	team = _team;
	type = _type;
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