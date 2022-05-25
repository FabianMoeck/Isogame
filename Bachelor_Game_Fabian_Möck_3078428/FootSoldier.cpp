#include "FootSoldier.h"

FootSoldier::FootSoldier(std::string _name, int _team) : ITroop(_name, _team)
{
	health = hp;
	speed = mS;

	attackSpeed = aS;
	damage = d;
	range = r;
	buildTime = bT;
}

FootSoldier::~FootSoldier()
{
}

void FootSoldier::onInit() {
	std::cout << "New " << name << " FS produced" << '\n';
}

void FootSoldier::onHit() {

}

void FootSoldier::onDeath() {
	std::cout << name << " died" << '\n';
}