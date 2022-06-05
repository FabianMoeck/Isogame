#include "../../Headers/Pathfinding/PathRequest.h"


PathRequest::PathRequest(GameObject* _move, glm::vec3 _targetPos, Pathfinding _finder, std::list<AttackRequest*>* _atkList)
{
	moving = true;
	moveGO = _move;
	targetPos = glm::vec3(_targetPos.x, _move->position.y, _targetPos.z);
	current = 0;
	atkList = _atkList;

	path = _finder.findPath(_move->position, targetPos);
}

PathRequest::PathRequest(GameObject* _move, GameObject* _targetObject, Pathfinding _finder, std::list<AttackRequest*> *_atkList) {
	targetObject = _targetObject;
	glm::vec3 _targetPos = findPosition(_move, _targetObject);
	atkList = _atkList;

	moveGO = _move;
	moving = true;
	targetPos = glm::vec3(_targetPos.x, _move->position.y, _targetPos.z);
	current = 0;

	path = _finder.findPath(_move->position, targetPos);
}

PathRequest::~PathRequest()
{
}

void PathRequest::move(float _deltaTime) {
	if (path.size() > 0 && moving) {
		glm::vec3* newTarget = new glm::vec3();
		if (current < path.size()) {
			//std::cout << current << "/" <<path.size() << " -- ";
			newTarget = get(&path, current);			//get current Path target
			newTarget = new glm::vec3(newTarget->x, 0.5f, newTarget->z);
		}

		if (atkList->size() > 0) {						//stop attacking when new Move command
			ITroop* t = (ITroop*)moveGO->u;
			for (AttackRequest* ar : *atkList) {
				if (ar->atk == t) {
					atkList->remove(ar);
					break;
				}
			}
		}

		if (current != path.size()) {							//current Target is not the last in Path
			float cDist = glm::distance(moveGO->position, *newTarget);		//get distance between current pos and Pathtarget
			if (cDist > 0.8f) {									//distance is gretaer than 0.8
				glm::vec3 newPos = glm::mix(moveGO->position, glm::vec3(newTarget->x, newTarget->y, newTarget->z), moveGO->u->speed * _deltaTime);			//move GO closer
				moveGO->position = glm::vec3(newPos.x, moveGO->position.y, newPos.z);
			}
			else {							//if distance is smaller than 0.8
				current++;					//add one to current pos
			}
		}
		else if (targetObject != nullptr) {
			moving = false;
			AttackRequest* atkRe = new AttackRequest((ITroop*)moveGO->u, targetObject->u);
			atkList->push_back(atkRe);
		}
		else {
			moving = false;
		}
	}
	else if (targetObject != nullptr) {
		moving = false;
		AttackRequest* atkRe = new AttackRequest((ITroop*)moveGO->u, targetObject->u);
		atkList->push_back(atkRe);
	}
	else {
		moving = false;
	}
}

glm::vec3 PathRequest::findPosition(GameObject* _move, GameObject* _target) {
	ITroop* t = (ITroop *)_move->u;
	float r = t->range;

	glm::vec3 result = glm::vec3();

	if (glm::distance(_move->position, _target->position) > r) {
		glm::vec3 dir = glm::normalize(_move->position - _target->position);
		glm::vec3 a = dir * r;

		result = _target->position + a;
	}
	else {
		result = _target->position;
	}

	return result;
}