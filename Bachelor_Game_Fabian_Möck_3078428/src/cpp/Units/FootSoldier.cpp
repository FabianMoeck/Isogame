#include "../../Headers/Units/FootSoldier.h"

FootSoldier::FootSoldier(std::string _name, int _team, int _type) : ITroop(_name, _team, _type)
{
	health = hp;
	speed = mS;

	attackSpeed = aS;
	damage = d;
	range = r;
	buildTime = bT;
	cost = c;

	onInit();
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