#ifndef MAINBUILDING_H
#define MAINBUILDING_H

#include "Building.h"

class MainBuilding : public Building
{
public:
	static float range;

	MainBuilding(std::string _name, int _team, int type);
	~MainBuilding();

	static void extendRange() {
		range += 1.0f;
	}

private:
	int hp = 350;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !MAINBUILDING_H

