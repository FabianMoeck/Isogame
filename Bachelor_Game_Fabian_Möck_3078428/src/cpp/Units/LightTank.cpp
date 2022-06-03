#include "../../Headers/Units/LightTank.h"

LightTank::LightTank(std::string _name, int _team, int _type) : ITroop(_name,_team, _type)
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

LightTank::~LightTank()
{
}

void LightTank::onInit() {
	std::cout << "New " << name << " LT produced" << '\n';
}

void LightTank::onHit() {

}

void LightTank::onDeath() {

}