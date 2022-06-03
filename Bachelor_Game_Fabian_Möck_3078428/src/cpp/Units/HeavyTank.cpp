#include "../../Headers/Units/HeavyTank.h"

HeavyTank::HeavyTank(std::string _name, int _team, int _type) : ITroop(_name, _team, _type)
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

HeavyTank::~HeavyTank()
{
}

void HeavyTank::onInit() {
	std::cout << "New " << name << " HT produced" << '\n';
}

void HeavyTank::onHit() {

}

void HeavyTank::onDeath() {

}