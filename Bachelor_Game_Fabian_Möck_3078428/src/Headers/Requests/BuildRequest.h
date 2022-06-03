#ifndef BUILDREQUEST_H
#define BUILDREQUEST_H

#include "../GameObject.h"
#include "../Scene.h"

class BuildRequest
{
public:
	bool handled = false;
	GameObject* parent;

	BuildRequest();
	BuildRequest(GameObject* _parent, std::string _name, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable, GameObject::GameObjectType _type, GameObject::Team _team);
	~BuildRequest();
	
	void build(Scene *scene, float *deltaTime);

	GameObject* getData();

private:
	GameObject* data;
	float timeBuilding = 0;
	
	glm::vec3 calcPosition(glm::vec3 _scale);
};

#endif // !BUILDREQUEST_H

