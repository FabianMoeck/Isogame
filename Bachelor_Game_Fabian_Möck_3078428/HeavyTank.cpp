#include "HeavyTank.h"

HeavyTank::HeavyTank(std::string _name, int _team) : ITroop(_name, _team)
{
	health = hp;
	speed = mS;

	attackSpeed = aS;
	damage = d;
	range = r;
	buildTime = bT;
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