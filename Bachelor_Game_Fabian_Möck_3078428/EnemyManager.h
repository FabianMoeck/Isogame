#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <list>
#include "src/Headers/GameObject.h"
#include "src/Headers/Requests/AttackRequest.h"
#include "src/Headers/Requests/BuildRequest.h"
#include "src/Headers/Pathfinding/PathRequest.h"
#include "src/Headers/Scene.h"

class EnemyManager
{
public:
	EnemyManager();
	EnemyManager(std::list<AttackRequest*>* _atk_list, std::list<BuildRequest*>* _build_list, std::list<PathRequest*>* _move_list, GameObject* _factory, GameObject* _barrack, Scene* _currentScene, Pathfinding* _pathfinder);
	~EnemyManager();

	void addMoney(float _deltaTime);

private:
	Scene* currentScene = nullptr;
	Pathfinding *pathfinder;

	std::list<AttackRequest*>* atk_list = nullptr;
	std::list<BuildRequest*>* build_list = nullptr;
	std::list<PathRequest*>* move_list = nullptr;

	GameObject* factory = nullptr;
	GameObject* barrack = nullptr;

	int currentMoney = 1500;
	float gatherSpeed = 1.5f;
	int gatherAmount = 30;
	float timeSinceLastAdd = 0.0f;

	int maxMoney = 5000;
	int unitAttackCount = 2;
	int minMoney = 200;

	std::list<GameObject*> currentUnits;

	void reduceMoney(int amount);
	void startProduce();
	void attackPlayer();

	GameObject* getRandomPlayerUnit();
	GameObject::GameObjectType getRandomType();

	GameObject* get(std::list<GameObject*>* _list, int _index) {
		std::list<GameObject*>::const_iterator it = _list->begin();
		for (unsigned int i = 0; i < _index; i++)
		{
			it++;
		}
		return *it;
	}
};

#endif // !ENEMY_MANAGER_H

