#ifndef MAINBUILDING_H
#define MAINBUILDING_H

#include "Building.h"

class MainBuilding : public Building
{
public:
	MainBuilding(std::string _name, int _team, int type);
	~MainBuilding();

private:
	int hp = 200;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !MAINBUILDING_H

