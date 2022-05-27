#include "AttackRequest.h"

AttackRequest::AttackRequest(ITroop* attacker, IUnit* target)
{
	atk = attacker;
	def = target;
}

AttackRequest::~AttackRequest()
{
}

int AttackRequest::attack(float _deltaTime) {
	if (timeSinceLastAttack > atk->attackSpeed) {
		def->takeDamage(atk->damage);
		def->onHit();
		if (def->hpState == IUnit::unitHealthState::dead) {
			if (def->team == 3 && def->type == 4) {
				def->onDeath();
				return 2;
			}
			else if (def->team == 1 && def->type == 4) {
				def->onDeath();
				return 3;
			}
			else {
				def->onDeath();
				return 0;
			}
		}
			
		timeSinceLastAttack = 0.0f;
	}
	else
		timeSinceLastAttack += _deltaTime;

	return 1;
}