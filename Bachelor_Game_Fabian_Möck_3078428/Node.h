#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>

class Node
{
public:
	glm::vec3 position;
	bool blocked;

	Node(glm::vec3 _position, bool _blocked);
	Node();
	~Node();

private:

};

#endif // !NODE_H

