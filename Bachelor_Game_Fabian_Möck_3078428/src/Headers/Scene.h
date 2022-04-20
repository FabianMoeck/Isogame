#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <glm/glm.hpp>
#include "GameObject.h"

class Scene
{
public:
	bool active;
	std::list<GameObject> SceneList;

	Scene();
	~Scene();
	GameObject getGameObject(std::list<GameObject> list, int index) {
		std::list<GameObject>::iterator it = list.begin();
		for (unsigned int i = 0; i < index; i++)
		{
			it++;
		}
		return *it;
	};

private:

};

#endif //SCENE_H