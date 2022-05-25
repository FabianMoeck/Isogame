#ifndef BARRACKS_H
#define BARRACKS_H

#include "Building.h"

class Barracks : public Building
{
public:
	Barracks(std::string _name, int _team);
	~Barracks();

private:
	int hp = 30;
	float bT = 5.5f;

	//std::list<GameObject::GameObjectType> bO = std::list<GameObject::GameObjectType>({ GameObject::GameObjectType::FootSoldier, GameObject::GameObjectType::Scout });

	void onInit();
	void onHit();
	void onDeath();
};

#endif // !BARRACKS_H

