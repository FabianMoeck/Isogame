#ifndef ATTACKREQUEST_H
#define ATTACKREQUEST_H

#include "../Units/Troop.h"
#include "../../../Collection.h"

class AttackRequest
{
public:
	ITroop* atk;
	IUnit* def;

	AttackRequest(ITroop *attacker, IUnit* target);
	~AttackRequest();

	int attack(float _deltaTime);

private:

	float timeSinceLastAttack = 0.0f;
};


#endif // !ATTACKREQUEST_H

