#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <glm/glm.hpp>
#include "../../Headers/Map.h"
#include <unordered_set>
#include <iostream>

class Pathfinding
{
public:
	Pathfinding() {};
	Pathfinding(Map *_map);
	~Pathfinding();

	std::list<glm::vec3*> findPath(glm::vec3 startPos, glm::vec3 targetPos);

private:
	Map *map;

	std::list<glm::vec3*> simplifiePath(std::list<Node*> *path);
	std::list<glm::vec3*> retracePath(Node* startNode, Node* targetNode);
	int getDistance(Node* a, Node* b);

	Node* get(std::list<Node*> _list, int _i) {
		std::list<Node*>::iterator it = _list.begin();
		for (int i = 0; i < _i; i++) {
			++it;
		}
		return *it;
	}
};

#endif // !PATHFINDING_H

