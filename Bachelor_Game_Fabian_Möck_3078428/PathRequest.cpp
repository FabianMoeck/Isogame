#include "PathRequest.h"

PathRequest::PathRequest(GameObject* _move, glm::vec3 _targetPos, Pathfinding _finder)
{
	moving = true;
	moveGO = _move;
	targetPos = glm::vec3(_targetPos.x, _move->position.y, _targetPos.z);

	path = _finder.findPath(_move->position, targetPos);
	std::cout << "Path found: \n";
	for (glm::vec3* v : path) {
		std::cout << v->x << "/" << v->z << '\n';
	}
	std::cout << '\n';
}

PathRequest::~PathRequest()
{
}

void PathRequest::move(float _deltaTime) {
	if (path.size() > 0 && moving) {

		glm::vec3* newTarget = get(path, current);						//get current Path target

		float cDist = glm::distance(moveGO->position, *newTarget);		//get distance between current pos and Pathtarget

		if (current != path.size()) {							//current Target is not the last in Path
			if (cDist > 0.8f) {									//distance is gretaer than 0.8
				glm::vec3 newPos = glm::mix(moveGO->position, glm::vec3(newTarget->x, newTarget->y, newTarget->z), _deltaTime);			//move GO closer
				moveGO->position = glm::vec3(newPos.x, moveGO->position.y, newPos.z);
			}
			else {							//if distance is smaller than 0.8
				current++;					//add one to current pos (theory next frame line 24)			//problem
			}
		}
		else {
			if (cDist > 0.8f) {
				glm::vec3 newPos = glm::mix(moveGO->position, targetPos, _deltaTime);				//move closer to target pos
				moveGO->position = glm::vec3(newPos.x, moveGO->position.y, newPos.z);
			}
			else {
				std::cout << "Moving false" << '\n';
				moving = false;										//stop moving
			}
		}
	}

	//working movement
	/*glm::vec3 newPos = glm::mix(moveGO->position, targetPos, _deltaTime);
	dist = glm::distance(newPos, targetPos);

	if (dist > 0.8) {
		moveGO->position = glm::vec3(newPos.x, targetPos.y, newPos.z);
	}
	else {
		moving = false;
	}*/

}