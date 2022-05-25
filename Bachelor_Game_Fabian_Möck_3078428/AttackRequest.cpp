#include "AttackRequest.h"

AttackRequest::AttackRequest(ITroop* attacker, IUnit* target)
{
	atk = attacker;
	def = target;
}

AttackRequest::~AttackRequest()
{
}

bool AttackRequest::attack(float _deltaTime) {
	if (timeSinceLastAttack > atk->attackSpeed) {
		def->takeDamage(atk->damage);
		def->onHit();
		if (def->hpState == IUnit::unitHealthState::dead) {
			def->onDeath();
			return false;
		}
			
		timeSinceLastAttack = 0.0f;
	}
	else
		timeSinceLastAttack += _deltaTime;

	return true;
}