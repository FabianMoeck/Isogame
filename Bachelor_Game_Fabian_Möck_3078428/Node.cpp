#include "Node.h"

Node::Node(glm::vec3 _position, bool _walkable, int _gridX, int _gridY)
{
	position = _position;
	walkable = _walkable;
	gridX = _gridX;
	gridY = _gridY;
}

Node::Node()
{
}

Node::~Node()
{
}