#include "Node.h"

Node::Node(glm::vec3 _position, bool _blocked)
{
	position = _position;
	blocked = _blocked;
}

Node::Node()
{
}

Node::~Node()
{
}