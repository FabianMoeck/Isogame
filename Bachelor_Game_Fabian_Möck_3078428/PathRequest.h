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
	bool moving;
	int current;

	PathRequest(GameObject* _move, glm::vec3 _targetPos, Pathfinding _finder);
	~PathRequest();

	void move(float _deltaTime);

	glm::vec3* get(std::list<glm::vec3*> *_list, int _i) {
		std::list<glm::vec3*>::iterator it = _list->begin();
		for (int i = 0; i < _i; i++) {
			++it;
		}
		return *it;
	}

private:
};

constexpr bool operator==(const PathRequest& n1, const PathRequest& n2)
{
	if (n1.moveGO == n2.moveGO)
		return true;
	else
		return false;
}

#endif // !PATHREQUEST_H

