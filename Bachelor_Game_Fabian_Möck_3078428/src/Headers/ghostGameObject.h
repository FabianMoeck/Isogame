#ifndef GHOST_H
#define GHOST_H

#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class GhostGO
{
public:
	std::string name;
	glm::vec3 scale;
	glm::vec3 color;
	bool selectable;
	GameObject::GameObjectType type;
	float angle;
	glm::vec3 tmp_position;

	GhostGO();
	GhostGO(std::string _name, glm::vec3 _scale, glm::vec3 _color, bool _selectable, GameObject::GameObjectType _type);
	~GhostGO();

	GameObject* placeGhost();
	void rotate(float deg);

private:

};

#endif // !GHOST_H
