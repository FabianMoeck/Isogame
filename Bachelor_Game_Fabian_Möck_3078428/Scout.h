#ifndef SCOUT_H
#define SCOUT_H

#include "Troop.h"

class Scout : public ITroop
{
public:
	Scout(std::string _name, int _team);
	~Scout();

private:
	int hp = 80;

	float mS = 1.2f;
	float r = 2.8f;
	int d = 10;
	float aS = 1.3f;

	virtual void onDeath();
	virtual void onInit();
	virtual void onHit();
};

#endif // !SCOUT_H

