#ifndef PATHREQUEST_H
#define PATHREQUEST_H

#include <list>
#include "glm/glm.hpp"
#include "src/Headers/GameObject.h"
#include "Pathfinding.h"

class PathRequest
{
public:
	std::list<glm::vec3*> path;
	GameObject* moveGO;
	glm::vec3 targetPos;
	float dist = 0;
	bool moving = true;

	PathRequest(GameObject* _move, glm::vec3 _targetPos, Pathfinding _finder);
	~PathRequest();

	void move(float _deltaTime);

private:

};

#endif // !PATHREQUEST_H

