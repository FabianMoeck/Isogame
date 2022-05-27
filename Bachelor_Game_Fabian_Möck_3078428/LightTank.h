#ifndef LTANK_H
#define LTANK_H

#include "Troop.h"

class LightTank : public ITroop
{
public:
	LightTank(std::string _name, int _team, int _type);
	~LightTank();

private:
	int hp = 100;
	float mS = 1.1f;
	float r = 2.6f;
	int d = 30;
	float aS = 2.0f;
	float bT = 1.0f;

	void onInit();
	void onHit();
	void onDeath();
};


#endif // !LTANK_H

