#ifndef SOLDIER_H
#define SOLDIER_H

#include "Troop.h"

class FootSoldier : public ITroop
{
public:
	int hp = 100;

	float mS = 1.0f;
	float r = 2.13f;
	int d = 15;
	float aS = 1.2f;
	float bT = 1.0f;

	FootSoldier(std::string _name, int _team);
	~FootSoldier();

private:

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !SOLDIER_H
