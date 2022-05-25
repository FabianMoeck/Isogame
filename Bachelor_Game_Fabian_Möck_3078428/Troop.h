#ifndef TROOP_H
#define TROOP_H

#include "IUnit.h"

class ITroop : public IUnit
{
public:
	float range = 0.0f;
	int damage = 0;
	float attackSpeed = 0.0f;

	ITroop(std::string _name, int _team);

protected:
	virtual ~ITroop();
};

#endif // !TROOP_H

