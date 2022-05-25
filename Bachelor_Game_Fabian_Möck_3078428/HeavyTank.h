#ifndef HTANK_H
#define HTANK_H

#include "Troop.h"

class HeavyTank : public ITroop
{
public:
	HeavyTank(std::string _name, int _team);
	~HeavyTank();

private:
	int hp = 150;
	float mS = 1.0f;
	float r = 2.0f;
	int d = 40;
	float aS = 2.3f;
	float bT = 1.0f;

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !HTANK_H

