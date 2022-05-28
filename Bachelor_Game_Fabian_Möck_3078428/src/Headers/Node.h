#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <iostream>

class Node
{
public:
	glm::vec3 position;
	bool walkable;
	int gridX;
	int gridY;

	int gCost;
	int hCost;
	int fCost() {
		return gCost + hCost;
	}

	Node* parent;

	Node(glm::vec3 _position, bool _walkable, int _gridX, int _gridY);
	Node();
	~Node();

private:

};

constexpr bool operator==(const Node& n1, const Node& n2)
{
	if (n1.position == n2.position)
		return true;
	else
		return false;
}

constexpr bool operator!=(const Node& n1, const Node& n2)
{
	return !(n1 == n2);
}

#endif // !NODE_H

