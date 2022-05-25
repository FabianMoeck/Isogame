#include "Factory.h"

Factory::Factory(std::string _name, int _team) : Building(_name, _team)
{
	health = hp;
	buildTime = bT;

	onInit();
}

Factory::~Factory()
{
}

void Factory::onInit() {

}

void Factory::onHit() {

}

void Factory::onDeath() {
	std::cout << name << " Destroyed\n";
}