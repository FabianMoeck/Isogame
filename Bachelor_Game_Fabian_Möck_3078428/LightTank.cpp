#include "LightTank.h"

LightTank::LightTank(std::string _name, int _team, int _type) : ITroop(_name,_team, _type)
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