#include "LightTank.h"

LightTank::LightTank(std::string _name, int _team) : ITroop(_name,_team)
{
	health = hp;
	speed = mS;

	attackSpeed = aS;
	damage = d;
	range = r;
	buildTime = bT;
}

LightTank::~LightTank()
{
}

void LightTank::onInit() {

}

void LightTank::onHit() {

}

void LightTank::onDeath() {

}