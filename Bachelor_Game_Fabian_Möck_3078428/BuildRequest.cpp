#include "BuildRequest.h"

BuildRequest::BuildRequest()
{
}

BuildRequest::BuildRequest(GameObject* _parent, std::string _name, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable, GameObject::GameObjectType _type, GameObject::Team _team) {
	parent = _parent;
	data = new GameObject(_name, calcPosition(_scale), _scale, _angle, _color, _selectable, _type, _team);
	parent->buildingQueue.push(data);
}

BuildRequest::~BuildRequest()
{
}

void BuildRequest::build(Scene* scene, float* deltaTime) {
	if (!parent->calculating) {
		parent->current = parent->buildingQueue.front();
		parent->calculating = true;
	}
	else {
		if (parent->current != nullptr && parent->current == data) {
			if (timeBuilding >= parent->current->u->buildTime) {
				IUnit* b = (IUnit*)parent->current->u;
				b->onInit();

				scene->SceneList.push_back(parent->current);
				handled = true;
				timeBuilding = 0.0f;
				parent->buildingQueue.pop();
				parent->calculating = false;
			}
			else {
				timeBuilding += *deltaTime;
			}
		}
	}
}

glm::vec3 BuildRequest::calcPosition(glm::vec3 _scale) {
	float x = parent->position.x + parent->scale.x;
	float y = parent->position.y;
	float z = parent->position.z;

	return glm::vec3(x,y,z);
}