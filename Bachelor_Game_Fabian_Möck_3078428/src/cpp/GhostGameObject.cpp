#include "../Headers/ghostGameObject.h"

GhostGO::GhostGO() {
	name = "";
	tmp_position = glm::vec3(0.0f, -10.0f, 0.0f);
}

GhostGO::GhostGO(std::string _name, glm::vec3 _scale, glm::vec3 _color, bool _selectable, GameObject::GameObjectType _type)
{
	name = _name;
	scale = _scale;
	color = _color;
	selectable = _selectable;
	type = _type;
	angle = 0.0f;
}

GhostGO::~GhostGO()
{
}

GameObject* GhostGO::placeGhost() {
	GameObject* newGO = new GameObject(name, tmp_position, scale, angle, color, selectable, type);
	return newGO;
}

void GhostGO::rotate(float deg) {
	angle += deg;
}