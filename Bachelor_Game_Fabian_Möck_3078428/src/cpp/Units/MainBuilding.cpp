#include "../../Headers/Units/MainBuilding.h"

MainBuilding::MainBuilding(std::string _name, int _team, int _type) : Building(_name, _team, _type)
{
	health = hp;

	onInit();
}

MainBuilding::~MainBuilding()
{
}

void MainBuilding::onInit()
{
}

void MainBuilding::onHit()
{

}

void MainBuilding::onDeath()
{
	std::cout << name << " Destroyed\n";
}