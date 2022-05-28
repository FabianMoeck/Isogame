#include "HeavyTank.h"

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

}

void HeavyTank::onHit() {

}

void HeavyTank::onDeath() {

}