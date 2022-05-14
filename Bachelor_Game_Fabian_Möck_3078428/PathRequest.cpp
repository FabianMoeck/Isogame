#include "PathRequest.h"

PathRequest::PathRequest(GameObject* _move, glm::vec3 _targetPos, Pathfinding _finder)
{
	moveGO = _move;
	targetPos = _targetPos;

	path = _finder.findPath(_move->position, _targetPos);
}

PathRequest::~PathRequest()
{
}

void PathRequest::move(float _deltaTime) {
	glm::vec3 newPos = glm::mix(moveGO->position, targetPos, _deltaTime);
	dist = glm::distance(newPos, targetPos);

	if (dist > 0.8) {
		moveGO->position = glm::vec3(newPos.x, targetPos.y, newPos.z);
	}
	else
		moving = false;
}